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
        socket.connect ("tcp://localhost:5555");
        
        zmq::message_t request (message.length());
        memcpy (request.data(), message.c_str(), message.length());
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        // RETURN THE REPLY FROM DAEMON
        
        return 0;
    }
    
    int PutAlert(std::string origin, std::string type, int subkey) {
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
    
    std::string GetAlert(std::string origin, std::string type, int subkey) {
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
        /*
        json j;
        j["operation"] = "get_alert";
        j["alert_key"]["origin"] = origin;
        j["alert_key"]["type"] = type;
        j["alert_key"]["subkey"] = subkey;
        SendToDaemon(j.dump());
        */
        
        //resAlert.Deserialize(str11);
        return str11;
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