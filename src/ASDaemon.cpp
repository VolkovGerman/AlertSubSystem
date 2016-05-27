#include <iostream>
#include <string>
#include <vector>
#include <zmq.hpp>

#include "Alert.hpp"
#include "DB.hpp"
#include "Email.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

int main() {
    
    std::cout << "Daemon started..." << std::endl;
    
    // ZeroMQ init
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555"); 
    
    while (1) {
        // Wait for the request from client
        zmq::message_t request;
        socket.recv (&request);
        std::string reqMessage = std::string(static_cast<char*>(request.data()), request.size());
        
        std::cout << "Request: " << reqMessage << std::endl;
        
        json jReq = json::parse(reqMessage);
        
        if (jReq["operation"] == "new_alert") {
            Email mail;
            
            Alert receivedAlert;
            receivedAlert.Deserialize(jReq["alert_key"].dump());
            
            std::cout << "Alert to email: " << receivedAlert.Serialize() << std::endl;
            
            mail.setRecipient("bylnov.nick@yandex.ru");
            mail.sendAlert(receivedAlert);
        }
        
        //  Send reply back to client
        zmq::message_t reply (4);
        memcpy (reply.data(), "Done", 4);
        socket.send (reply);
    }
    
    return 0;
}