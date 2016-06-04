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

std::queue<std::string> messagesQueue;
std::queue<Alert> alertQueue;

int FillAlertFields(Alert &newAlert) {
    std::string origin = newAlert.get_origin();
    std::string type = newAlert.get_type();
    std::string subkey = newAlert.get_subkey_string();
    
    newAlert.set_priority(conf.GetPriority(origin, type, subkey));
    newAlert.set_severity(conf.GetSeverity(origin, type, subkey));
    newAlert.set_message(conf.GetMessage(origin, type, subkey));
    
    return 0;
}

void * messagesQueue_processing(void *message) {

    while (1){
        while (!messagesQueue.size()) {};
        
        std::string reqMessage = messagesQueue.front();
        messagesQueue.pop();
        json jReq = json::parse(reqMessage);
        
        Alert receivedAlert;
        receivedAlert.Deserialize(jReq["alert_key"].dump());
        
        if (jReq["operation"] == "new_alert") {
        
            // Watch fields from config, write them to alert object
            FillAlertFields(receivedAlert);
            
            // If mode is immidiate - send email
            int mode = 0;
            if (conf.GetMode() == "im") {
                // send email
                Email mail;
                
                std::cout << "Alert to email: " << receivedAlert.Serialize() << std::endl;
                
                mail.setRecipient("volkov.german.1997@gmail.com");
                mail.SendAlert(receivedAlert);
            } else {
                alertQueue.push(receivedAlert);
            }
        
        }
    }   
    
    return message;
}

void * alertQueue_processing(void *message) {
    
    while (1){
        std::cout << "Alert Queue Thread - Start sleeping 30 seconds" << std::endl;
        sleep(30);
        std::cout << "Alert Queue Thread - End sleeping 30 seconds" << std::endl;
        
        std::cout << "Alert Queue Thread - Watching queue..." << std::endl;
        std::cout << "Alert Queue Thread - There are " << alertQueue.size() << " new alerts in queue!" << std::endl;
        
        std::vector<Alert> alertsToSend;
        while(!alertQueue.empty()) {
            Alert alertFromQueue = alertQueue.front();
            alertQueue.pop();
                
            // If periodicity is okay - send, and then change state of the alert in database
            alertsToSend.push_back(alertFromQueue);
        }        
        
        if (alertsToSend.size() != 0){
            // Show all alerts
            std::cout << alertsToSend.size() << std::endl;
            for (int i = 0; i < alertsToSend.size(); i++){
                std::cout << alertsToSend[i].Serialize() << std::endl;
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
    
    pthread_t th1, th2;
    
    pthread_create(&th1, NULL, messagesQueue_processing, (void *)message);
    std::cout << conf.GetMode() << std::endl;
    if (conf.GetMode() == "period"){
        pthread_create(&th2, NULL, alertQueue_processing, (void *)message);
    }
    
    std::cout << "Daemon started..." << std::endl;
    
    // ZeroMQ init
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:55555"); 
    
    while (1) {
        // Wait for the request from client
        zmq::message_t request;
        socket.recv(&request);
        std::string reqMessage = std::string(static_cast<char*>(request.data()), request.size());
        
        messagesQueue.push(reqMessage);
        
        //  Send reply back to client
        zmq::message_t reply (4);
        memcpy (reply.data(), "Done", 4);
        socket.send (reply); 
    }
    
    pthread_join(th1, NULL);
    if (conf.GetMode() == "period"){
        pthread_join(th2, NULL);
    }
    
    return 0;
}