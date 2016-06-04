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

int FillAlertFields(Alert &newAlert) {
    std::string origin = newAlert.get_origin();
    std::string type = newAlert.get_type();
    std::string subkey = newAlert.get_subkey();
    
    newAlert.set_priority(conf.GetPriority(origin, type, subkey));
    newAlert.set_severity(conf.GetSeverity(origin, type, subkey));
    newAlert.set_message(conf.GetMessage(origin, type, subkey));
    
    return 0;
}

std::string MessagesProcessing(std::string reqMessage) {
    std::string result;
    json jReq = json::parse(reqMessage);
    
    //std::cout << "4444444444";
    
    Alert receivedAlert;
    receivedAlert.Deserialize(jReq["alert_key"].dump());
    
    //std::cout << "333333333";
    
    // Watch fields from config, write them to alert object
    FillAlertFields(receivedAlert);
    
    std::cout << "----" << receivedAlert.SerializeValue() << "----";
    
    //std::cout << receivedAlert.SerializeValue();
    
    //std::cout << "111111111";
    
    if (jReq["operation"] == "new_alert") {    
        // If mode is immidiate - send email
        int mode = 0;
        if (conf.GetMode() == "immidiatly") {
            // send email
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
        
        j["message"] = receivedAlert.get_message();
        j["priority"] = receivedAlert.get_priority_string();
        j["severity"] = receivedAlert.get_severity_string();
        
        //result = database.Get(j.dump()); 
        result = j.dump();
        //std::cout << result << "0000000000";
    }
    
    return result;
}

void * alertQueue_processing(void *message) {
    int max_alerts_per_mail = conf.GetMaxAlerts();
    
    while (1){
        std::cout << "Alert Queue Thread - Start sleeping 30 seconds" << std::endl;
        sleep(conf.GetPeriodisityTime());
        std::cout << "Alert Queue Thread - End sleeping 30 seconds" << std::endl;
        
        std::cout << "Alert Queue Thread - Watching queue..." << std::endl;
        std::cout << "Alert Queue Thread - There are " << alertQueue.size() << " new alerts in queue!" << std::endl;
        
        std::vector<Alert> alertsToSend;
        int alertCount = max_alerts_per_mail;
        while(!alertQueue.empty() && alertCount--) {
            Alert alertFromQueue = alertQueue.front();
            alertQueue.pop();
                
            // If periodicity is okay - send, and then change state of the alert in database
            alertsToSend.push_back(alertFromQueue);
        }        
        
        if (alertsToSend.size() != 0){
            // Show all alerts
            std::cout << alertsToSend.size() << std::endl;
            for (int i = 0; i < alertsToSend.size(); i++){
                std::cout << alertsToSend[i].SerializeKey() << std::endl;
            }
        
            Email mail;
            mail.setRecipient("volkov.german.1997@gmail.com");
            mail.SendAlerts(alertsToSend);
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
    socket.bind ("tcp://*:7777"); 
    
    while (1) {
        // Wait for the request from client
        zmq::message_t request;
        socket.recv(&request);
        std::string reqMessage = std::string(static_cast<char*>(request.data()), request.size());
        
        std::string res1 = MessagesProcessing(reqMessage);
        std::cout << res1;
        
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