#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "include/alert.h"
#include "include/bd.h"

int ParseRequest(std::string reqString, alert* receivedAlert){
    
    int mode = (int)reqString[0];
    
    receivedAlert->k.origin = reqString;
    
    return mode;
}

int sendEmail(alert alertData){
    return 0;
}

int sendBack(std::string mail){
    return 0;
}

int main(){
    
    // Variables init
    std::string requestString = "daw";
    alert receivedAlert;
    bd database;
    alert *listOfRequestedAlerts;
    
    // Components init section
    
    while (1){
        // Read by ZeroMQ on port
        
        int mode = ParseRequest(requestString, &receivedAlert); 
        
        int receivedFromBD = 0;
        if (mode >= 0 && mode <= 19) {
            receivedFromBD = database.put(receivedAlert, listOfRequestedAlerts);
        } else if (mode >= 20 && mode <= 39) {
            receivedFromBD = database.get(receivedAlert, listOfRequestedAlerts);
        } else if (mode >= 40 && mode <= 59) {
            receivedFromBD = database.remove(receivedAlert, listOfRequestedAlerts);
        }
        
        // Serialize listOfRequestedAlerts, which will be send back
        std::string res = serialize(listOfRequestedAlerts); 
        sendBack(res);
        
        switch (receivedFromBD){
            case 0:
                std::cout << "Error: There was an error working with bd" << std::endl;
                break;
            case 1:
                // Record exist(s), email will NOT be sent
                
                break;
            case 2:
                // Record exist(s), email will be sent
                
                sendEmail(receivedAlert);
                break;
            case 3:
                // Record do not exist, email will NOT be sent
                
                break;
            case 4:
                // Record do not exist, email will be sent
                
                sendEmail(receivedAlert);
                break;
            default: 
                cout << "Error: Wrong value was received from database."
                break;
        }
        
        // Trace info and sleep
        // WE NEED LOGGER!
        std::cout << receivedAlert.k.origin << std::endl;
        sleep(1);
    }
    
    getchar();
    return 0;
}