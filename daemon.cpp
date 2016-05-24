#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <zmq.hpp>
#include <string>
#include <vector>
#include <unistd.h>
#include "include/bd.h"
#include "include/email.h"
#include "include/alert.h"

int main(int argc, char* argv[]){
    // Variables init
    std::string requestString = "daw";
    alert receivedAlert;
    bd database;
    std::vector<alert> listOfRequestedAlerts;  
    email mail;
    
    mail.setRecipient("Volkov.german.1997@gmail.com"); 
    
    // ZeroMQ init
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555"); 
    
    while (1){
        // Wait for the request from client
        zmq::message_t request;
        socket.recv (&request);
        std::string reqMessage = std::string(static_cast<char*>(request.data()), request.size());
        
        std::cout << reqMessage << std::endl;
        
        // Parse alert from request message
        int mode = receivedAlert.initFromString(reqMessage); 
        
        std::cout << "dawadwadwdawd" << std::endl;
        
        int receivedFromBD = 0;
        switch (mode) {
            case 50:
                receivedFromBD = database.put(receivedAlert);
                break;
            case 51:
                // receivedFromBD = database.get(receivedAlert, listOfRequestedAlerts);
                listOfRequestedAlerts = database.get(receivedAlert);
                (listOfRequestedAlerts.empty()) ? receivedFromBD = 3 : receivedFromBD = 1;
                break;
            case 52:
                receivedFromBD = database.remove(receivedAlert);
                break;
        }
        
        std::cout << "---" << receivedFromBD << std::endl;
        
        // Serialize listOfRequestedAlerts, which will be send back
        //std::string res = serialize(listOfRequestedAlerts); 
        //sendBack(res);
        
        switch (receivedFromBD){
            case 0:
                // Error: There was an error working with bd
                // It's time for logger
                
                break;
            case 1:
                // Record exist(s), email will NOT be sent
                // It's time for logger
                
                // - временно для демонстрации
                mail.sendAlert(receivedAlert);
                // - удалить как только появится возможность
                
                break;
            case 2:
                // Record exist(s), email will be sent
                // It's time for logger

                
                mail.sendAlert(receivedAlert);

                break;
            case 3:
                // Record do not exist, email will NOT be sent
                // It's time for logger
                
                break;
            case 4:
                // Record do not exist, email will be sent
                // It's time for logger

                mail.sendAlert(receivedAlert);
                
                break;
            default: 
                std::cout << "Error: Wrong value was received from database." << std::endl;
                break;
        }
        
        //  Send reply back to client
        zmq::message_t reply (4);
        memcpy (reply.data(), "Done", 4);
        socket.send (reply);
    }
    
    getchar();
    return 0;
}
