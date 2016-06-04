#pragma once

#include <vector>
#include <zmq.hpp>

#include "Alert.hpp"
#include "DB.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

namespace AlertSubSystem{
    
    int SendToDaemon(std::string message){
        //  Prepare our context and socket
        zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_REQ);
        socket.connect ("tcp://localhost:4444");
        
        zmq::message_t request (message.length());
        memcpy (request.data(), message.c_str(), message.length());
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::string replyMessage = std::string(static_cast<char*>(reply.data()), reply.size());
        
        return replyMessage;
    }
    
    int PutAlert(std::string origin, std::string type, std::string subkey) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        
        // Put alert to bd
        database.Put(reqAlert.Serialize());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j, j_alert;
        j["operation"] = "new_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        SendToDaemon(j.dump());
        
        return 0;
    }
    
    Alert GetAlert(std::string origin, std::string type, std::string subkey) {
        Alert reqAlert(origin, type, subkey);
        Alert resAlert;
        DB database;
        
        // Put alert to bd
        json j;
        j["origin"] = origin;
        j["type"] = type;
        j["subkey"] = subkey;
        std::string str11 = database.Get(j.dump());
        
        // Send message to daemon: operation number, alert key
        json j;
        j["operation"] = "get_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        std::string replyStr = SendToDaemon(j.dump());
        resAlert = json::parse(replyStr);
        
        return resAlert;
    }
    
    std::vector<Alert> GetAlerts(std::string origin, std::string type) {
        Alert reqAlert(origin, type);
        std::vector<Alert> resAlerts;
        
        // Send message to daemon: operation number, alert key
        json j;
        j["operation"] = "get_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        SendToDaemon(j.dump());
        
        return resAlerts;
    }
    
    std::vector<Alert> GetAlerts(std::string origin) {
        Alert reqAlert(origin);
        std::vector<Alert> resAlerts;
        
        // Send message to daemon: operation number, alert key
        json j;
        j["operation"] = "get_alert";
        j["alert_key"]["origin"] = origin;
        SendToDaemon(j.dump());
        
        return resAlerts;
    } 
    
}