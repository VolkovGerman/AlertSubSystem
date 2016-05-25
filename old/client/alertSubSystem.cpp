#include <iostream>
#include <string>
#include <vector>
#include "alertSubSystem.h"
#include "alert.h"
#include <zmq.hpp>

int AlertSubSystem::sendToDaemon(std::string message){
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    socket.connect ("tcp://localhost:5555");
    
    zmq::message_t request (message.length());
    memcpy (request.data(), message.c_str(), message.length());
    socket.send (request);

    //  Get the reply.
    zmq::message_t reply;
    socket.recv (&reply);
    
    return 0;
}

std::vector<alert> AlertSubSystem::get_alert(std::string origin) { 
    
    alert buf; 
    std::vector<alert> vec; 
    std::string info = buf.serialize('3'); 
    
    AlertSubSystem::sendToDaemon(info);
    
    return vec; 
}

int AlertSubSystem::put_alert(){ 

    alert newAlert; 
    std::string info = newAlert.serialize('2'); 
    
    AlertSubSystem::sendToDaemon(info);
    
    return 0; 
}

