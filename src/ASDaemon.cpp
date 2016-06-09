#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <zmq.hpp>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "Alert.hpp"
#include "DB.hpp"
#include "Email.hpp"
#include "Config.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

Config conf;

std::queue<Alert> alertQueue;

std::string Trim(std::string str) {
    return str.substr(1, str.length() - 2);
}

int FillAlertFields(Alert &newAlert) {
    std::string origin = newAlert.get_origin();
    std::string type = newAlert.get_type();
    std::string subkey = newAlert.get_subkey();

    if (newAlert.get_priority() == NO_PR) {
        newAlert.set_priority(conf.GetPriority(origin, type, subkey));
    }
    if (newAlert.get_severity() == NO_SV) {
        newAlert.set_severity(conf.GetSeverity(origin, type, subkey));
    }
    if (newAlert.get_message() == "") {
        newAlert.set_message(Trim(conf.GetMessage(origin, type, subkey)));
    }
    
    return 0;
}

std::string MessagesProcessing(std::string reqMessage) {
    std::string result;
    json jReq = json::parse(reqMessage);
    
    Alert receivedAlert;
    receivedAlert.Deserialize(jReq["alert_key"].dump());

    if (jReq["operation"] == "new_alert") {  
        // Watch fields from config, write them to alert object
        FillAlertFields(receivedAlert); 

        // If mode is immidiate - send email
        int mode = 0;
        if (conf.GetMode() == "immidiatly") {
            // Send email
            Email mail;
            
            std::cout << "Alert to email: " << receivedAlert.SerializeKey() << std::endl;
            
            mail.setRecipient("volkov.german.1997@gmail.com");
            mail.SendAlert(receivedAlert);
        } else {
            alertQueue.push(receivedAlert);
        }
        
        result = "Done";
    } else if (jReq["operation"] == "get_alert") {
        DB database;
        json j;
        j["origin"] = receivedAlert.get_origin();
        j["type"] = receivedAlert.get_type();
        j["subkey"] = receivedAlert.get_subkey();

        std::cout << "GERA1" << std::endl;
        
        // Get Message
        if (database.GetMessage(jReq["alert_key"].dump()) != "") {
            j["message"] = database.GetMessage(jReq["alert_key"].dump());
        } else {
            j["message"] = Trim(conf.GetMessage(receivedAlert.get_origin(), receivedAlert.get_type(), receivedAlert.get_subkey()));
        }

         std::cout << "GERA2" << std::endl;

        // Get Priority
        if (database.GetPriority(jReq["alert_key"].dump()) != " ") {
            j["priority"] = database.GetPriority(jReq["alert_key"].dump());
        } else {
            j["priority"] = Trim(conf.GetStrPriority(receivedAlert.get_origin(), receivedAlert.get_type(), receivedAlert.get_subkey()));
        }

        std::cout << "GERA3" << std::endl;

        // Get Severity
        if (database.GetSeverity(jReq["alert_key"].dump()) != "") {
            j["severity"] = database.GetSeverity(jReq["alert_key"].dump());
        } else {
            j["severity"] = Trim(conf.GetStrSeverity(receivedAlert.get_origin(), receivedAlert.get_type(), receivedAlert.get_subkey()));
        }
        
        result = database.Get(j.dump()); 
        result = j.dump();
    }
    
    return result;
}

void *alertQueue_processing(void *message) {
    DB database;
    std::vector<Alert> alertsToSend;
    int max_alerts_per_mail = conf.GetMaxAlerts();
    
    while (1){
        std::cout << "Alert Queue Thread - Start sleeping " << conf.GetPeriodisityTime() << " seconds" << std::endl;
        sleep(conf.GetPeriodisityTime());
        std::cout << "Alert Queue Thread - End sleeping" << std::endl;
        
        // Is there something in queue ?
        std::cout << "Alert Queue Thread - Watching queue..." << std::endl;
        std::cout << "Alert Queue Thread - There are " << alertQueue.size() << " new alerts in queue!" << std::endl;
        std::cout << "Alert Queue Thread - There are " << alertsToSend.size() << " old alerts in queue!" << std::endl;

        int alertCount = max_alerts_per_mail - alertsToSend.size();
        //std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<" << max_alerts_per_mail << std::endl;
        while(!alertQueue.empty() && alertCount--) {
            Alert alertFromQueue = alertQueue.front();
            
            // If periodicity is okay - add to vector, and then change state of the alert in database

            // Check periodicity
            time_t alertPeriodicity = conf.GetPeriodisityTime(alertFromQueue.get_origin(), alertFromQueue.get_type(), alertFromQueue.get_subkey());
            
            std::cout << "---------------------------------" << std::endl;
            std::cout << alertPeriodicity << std::endl;
            std::cout << "---------------------------------" << std::endl;
            std::cout << alertFromQueue.get_creation_time() << std::endl;
            std::cout << "---------------------------------" << std::endl;
            time_t time1 = time(0);
            std::cout << time1 << std::endl;
            std::cout << "---------------------------------" << std::endl;

            json jAlertFields;
            jAlertFields["origin"] = alertFromQueue.get_origin();
            jAlertFields["type"] = alertFromQueue.get_type();
            jAlertFields["subkey"] = alertFromQueue.get_subkey();

            if (database.HasOldActiveAlert(jAlertFields.dump())) {
                if (database.GetCreationTime(jAlertFields.dump()) + alertPeriodicity > time(0)) {
                    std::cout << "Alert -- For this alert periodicity time has NOT passed" << std::endl;
                } else {
                    alertsToSend.push_back(alertFromQueue); 
                    database.SetCreationTime(jAlertFields.dump(), alertFromQueue.get_creation_time());
                    database.SetState(jAlertFields.dump(), "UNSTABLE");
                }
            } else {
                alertsToSend.push_back(alertFromQueue);   
                database.SetState(jAlertFields.dump(), "CLEARED");
            }

            // Pop this alert out
            alertQueue.pop();             
        }   

        int resOfSending = 1;     
        
        if (alertsToSend.size() != 0){
            // Show all alerts
            // std::cout << alertsToSend.size() << std::endl;
            for (int i = 0; i < alertsToSend.size(); i++){
                std::cout << alertsToSend[i].SerializeKey() << std::endl;
            }
        
            Email mail;
            mail.setRecipient("volkov.german.1997@gmail.com");
            resOfSending = mail.SendAlerts(alertsToSend);
        }

        if (resOfSending == 0) {  
            // Everestring is okay - empty alertsToSend
            alertsToSend.clear();
        }
        
        
    }
 
    return message;   
}

int main() {
    
    const char* message = "Heeelllooo!";
    
    pthread_t th1;
    
    
    std::cout << conf.GetMode(); 
    if (conf.GetMode() == "batch"){
        pthread_create(&th1, NULL, alertQueue_processing, (void *)message);
    }

    std::cout << "Daemon started..." << std::endl;
    
    // ZeroMQ init
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:3333"); 
    
    while (1) {
        // Wait for the request from client
        zmq::message_t request;
        socket.recv(&request);
        std::string reqMessage = std::string(static_cast<char*>(request.data()), request.size());
        
        std::string res1 = MessagesProcessing(reqMessage);
        std::cout << "LIMONAD" << res1;
        
        //  Send reply back to client
        zmq::message_t reply (res1.length());
        memcpy (reply.data(), res1.c_str(), res1.length());
        socket.send (reply);
    }
    
    if (conf.GetMode() == "batch") {
        pthread_join(th1, NULL);
    }
    
    return 0;
}