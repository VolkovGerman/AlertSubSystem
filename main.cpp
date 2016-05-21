#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "include/alert.h"

int ParseRequest(std::string reqString, alert* receivedAlert){
    
    receivedAlert->k.origin = reqString;
    
    return 1;
}

int main(){
    
    // Variables init
    std::string requestString = "daw";
    alert receivedAlert;
    
    // Components init section
    
    while (1){
        // Read by ZeroMQ on port
        
        ParseRequest(requestString, &receivedAlert); 
        
        std::cout << receivedAlert.k.origin << std::endl;
        
        sleep(1);
    }
    
    getchar();
    return 0;
}