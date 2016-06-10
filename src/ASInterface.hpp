#pragma once

#include <vector>
#include <zmq.hpp>

#include "Alert.hpp"
#include "DB.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

namespace AlertSubSystem{
    
    std::string SendToDaemon(std::string message){
        //  Prepare our context and socket
        zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_REQ);
        socket.connect ("tcp://localhost:3333");
        
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
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        j["alert_key"]["creation_time"] = reqAlert.get_creation_time();
        j["alert_key"]["state"] = "NEW";
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, std::string message) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();
        jAlertToDB["message"] = message;
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        j["alert_key"]["creation_time"] = reqAlert.get_creation_time();
        j["alert_key"]["message"] = message;
        j["alert_key"]["state"] = "NEW";
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Severity sv) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();

        switch (sv) {
            case CRITICAL:
                jAlertToDB["severity"] = "CRITICAL";
                break;
            case ERROR:
                jAlertToDB["severity"] = "ERROR";
                break;
            case WARNING:
                jAlertToDB["severity"] = "WARNING";
                break;
            case DEBUG:
                jAlertToDB["severity"] = "DEBUG";
                break;
            case NO_SV:
                jAlertToDB["severity"] = "NO_SV";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Priority pr) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();

        switch (pr) {
            case HIGH:
                jAlertToDB["priority"] = "HIGH";
                break;
            case MEDIUM:
                jAlertToDB["priority"] = "MEDIUM";
                break;
            case LOW:
                jAlertToDB["priority"] = "LOW";
                break;
            case NO_PR:
                jAlertToDB["priority"] = "NO_PR";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Priority pr, Severity sv) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();

        switch (sv) {
            case CRITICAL:
                jAlertToDB["severity"] = "CRITICAL";
                break;
            case ERROR:
                jAlertToDB["severity"] = "ERROR";
                break;
            case WARNING:
                jAlertToDB["severity"] = "WARNING";
                break;
            case DEBUG:
                jAlertToDB["severity"] = "DEBUG";
                break;
            case NO_SV:
                jAlertToDB["severity"] = "NO_SV";
                break;
        }

        switch (pr) {
            case HIGH:
                jAlertToDB["priority"] = "HIGH";
                break;
            case MEDIUM:
                jAlertToDB["priority"] = "MEDIUM";
                break;
            case LOW:
                jAlertToDB["priority"] = "LOW";
                break;
            case NO_PR:
                jAlertToDB["priority"] = "NO_PR";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Severity sv, Priority pr) {
        PutAlert(origin, type, subkey, pr, sv);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, std::string message, Priority pr) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();
        jAlertToDB["message"] = message;

        switch (pr) {
            case HIGH:
                jAlertToDB["priority"] = "HIGH";
                break;
            case MEDIUM:
                jAlertToDB["priority"] = "MEDIUM";
                break;
            case LOW:
                jAlertToDB["priority"] = "LOW";
                break;
            case NO_PR:
                jAlertToDB["priority"] = "NO_PR";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, std::string message, Severity sv) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();
        jAlertToDB["message"] = message;

        switch (sv) {
            case CRITICAL:
                jAlertToDB["severity"] = "CRITICAL";
                break;
            case ERROR:
                jAlertToDB["severity"] = "ERROR";
                break;
            case WARNING:
                jAlertToDB["severity"] = "WARNING";
                break;
            case DEBUG:
                jAlertToDB["severity"] = "DEBUG";
                break;
            case NO_SV:
                jAlertToDB["severity"] = "NO_SV";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Priority pr, std::string message) {
        PutAlert(origin, type, subkey, message, pr);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Severity sv, std::string message) {
        PutAlert(origin, type, subkey, message, sv);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, std::string message, Severity sv, Priority pr) {
        Alert reqAlert(origin, type, subkey);
        DB database;
        json jAlertToDB;

        // Put alert to bd
        jAlertToDB["origin"] = origin;
        jAlertToDB["type"] = type;
        jAlertToDB["subkey"] = subkey;
        jAlertToDB["creation_time"] = reqAlert.get_creation_time();
        jAlertToDB["message"] = message;

        switch (sv) {
            case CRITICAL:
                jAlertToDB["severity"] = "CRITICAL";
                break;
            case ERROR:
                jAlertToDB["severity"] = "ERROR";
                break;
            case WARNING:
                jAlertToDB["severity"] = "WARNING";
                break;
            case DEBUG:
                jAlertToDB["severity"] = "DEBUG";
                break;
            case NO_SV:
                jAlertToDB["severity"] = "NO_SV";
                break;
        }

        switch (pr) {
            case HIGH:
                jAlertToDB["priority"] = "HIGH";
                break;
            case MEDIUM:
                jAlertToDB["priority"] = "MEDIUM";
                break;
            case LOW:
                jAlertToDB["priority"] = "LOW";
                break;
            case NO_PR:
                jAlertToDB["priority"] = "NO_PR";
                break;
        }
        
        jAlertToDB["state"] = "NEW";
        database.Put(jAlertToDB.dump());
        
        // Send message to daemon, which contains alert key and operation number (0) 
        json j;
        j["operation"] = "new_alert";
        j["alert_key"] = jAlertToDB;
        SendToDaemon(j.dump());

        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, std::string message, Priority pr, Severity sv) {
        PutAlert(origin, type, subkey, message, sv, pr);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Severity sv, std::string message, Priority pr) {
        PutAlert(origin, type, subkey, message, sv, pr);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Severity sv, Priority pr, std::string message) {
        PutAlert(origin, type, subkey, message, sv, pr);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Priority pr, Severity sv, std::string message) {
        PutAlert(origin, type, subkey, message, sv, pr);
        return 0;
    }

    int PutAlert(std::string origin, std::string type, std::string subkey, Priority pr, std::string message, Severity sv) {
        PutAlert(origin, type, subkey, message, sv, pr);
        return 0;
    }

    Alert GetAlert(std::string origin, std::string type, std::string subkey) {
        Alert reqAlert(origin, type, subkey);
        Alert resAlert;
        DB database;
        
        // Put alert to bd
        json j;
        j["operation"] = "get_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        
        std::string replyStr = SendToDaemon(j.dump());
        resAlert.Deserialize(replyStr);
        
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