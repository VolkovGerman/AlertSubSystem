#pragma once

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

#include "json.hpp"

// for convenience
using json = nlohmann::json;

enum Priority {
	HIGH,
	MEDIUM,
	LOW
};

enum Severity {
	CRITICAL,
	ERROR,
	WARNING,
	DEBUG
};

enum State {
    NEW,
    CLEARED,
    UNSTABLE,
    PROCESSED
};

struct Key {
	std::string origin;
	std::string type;
	std::string subkey;
};

struct Value {	
	time_t creation_time;
	long periodicity_time;
	std::string message;
    std::vector<std::string> recipients_email;
	State state;
	Priority priority;
	Severity severity;
};

class Alert {
    
    private:
        struct Key alert_key_;
        struct Value alert_value_;
    
    public:
        
        // Constructors
        Alert(){};
        
        Alert(std::string origin, std::string type, int subkey) {
            alert_key_.origin = origin;
            alert_key_.type = type;
            alert_key_.subkey = subkey;
        }
        
        Alert(std::string origin, std::string type) {
            alert_key_.origin = origin;
            alert_key_.type = type;
        }
        
        Alert(std::string origin) {
            alert_key_.origin = origin;
        }
        
        // Get / set methods
        std::string get_origin(){
            return alert_key_.origin;
        }
        
        int set_origin(std::string str){
            alert_key_.origin = str;
            return 0;
        }  
        
        std::string get_type(){
            return alert_key_.type;
        }
        
        int set_type(std::string str){
            alert_key_.type = str;
            return 0;
        }  
        
        std::string get_subkey(){
            return alert_key_.subkey;
        }
        
        int set_subkey(int num){
            alert_key_.subkey = num;
            return 0;
        }  
        
        State get_state(){
            return alert_value_.state;
        }
        
        int set_state(State val){
            alert_value_.state = val;
            return 0;
        }
        
        Priority get_priority(){
            return alert_value_.priority;
        }
        
        std::string get_priority_string(){
            switch (alert_value_.priority) {
                case HIGH:
                    return "HIGH";
                case MEDIUM:
                    return "MEDIUM";
                case LOW:
                    return "LOW";
                default:
                    return "Wrong priority...";
            }
            
            return NULL;
        }
        
        int set_priority(Priority val){
            alert_value_.priority = val;
            return 0;
        }
        
        int set_priority_from_string(std::string pr) { 
             if (pr == "HIGH") {
                this->alert_value_.priority = HIGH;
            } else if (pr == "MEDIUM") {
                this->alert_value_.priority = MEDIUM;
            } else if (pr == "LOW") {
                this->alert_value_.priority = LOW;
            }
            
            return 0;
        }
        
        Severity get_severity(){
            return alert_value_.severity;
        }
        
        std::string get_severity_string(){
            switch (alert_value_.severity) {
                case CRITICAL:
                    return "CRITICAL";
                case ERROR:
                    return "ERROR";
                case WARNING:
                    return "WARNING";
                case DEBUG:
                    return "DEBUG";
                default:
                    return "Wrong severity...";
            }
            
            return NULL;
        }
        
        int set_severity(Severity val){
            alert_value_.severity = val;
            return 0;
        }
        
        int set_severity_from_string(std::string sv) { 
            if (sv == "CRITICAL") {
                this->alert_value_.severity = CRITICAL;
            } else if (sv == "ERROR") {
                this->alert_value_.severity = ERROR;
            } else if (sv == "WARNING") {
                this->alert_value_.severity = WARNING;
            } else if (sv == "DEBUG") {
                this->alert_value_.severity = DEBUG;
            }
            
            return 0;
        }
        
        std::string get_message() {
            return alert_value_.message;
        }
        
        int set_message(std::string str) {
            alert_value_.message = str;
            return 0;
        }
        
        std::vector<std::string> get_recipients_email() {
            return alert_value_.recipients_email;
        }
        
        int set_recipients_email(std::vector<std::string> vectEmails) {
            alert_value_.recipients_email = vectEmails;
            return 0;
        }
        
        long get_periodicity_time(){
            return alert_value_.periodicity_time;
        }
        
        int set_periodicity_time(long t) {
            alert_value_.periodicity_time = t;
            return 0;
        }
        
        time_t get_creation_time(){
            return alert_value_.creation_time;
        }
        
        std::string get_creation_time_pretty() {   
            time_t now = time(0);
            tm *ltm = localtime(&now);
            std::stringstream date;
            date << ltm->tm_mday
                << "/"
                << 1 + ltm->tm_mon
                << "/"
                << 1900 + ltm->tm_year
                << " "
                << 1 + ltm->tm_hour
                << ":"
                << 1 + ltm->tm_min
                << ":"
                << 1 + ltm->tm_sec;
            std::cout << date.str() << "\n";
            
            return date.str();
        }
        
        // Serialize / Deserialize
        
        std::string Serialize(){
            json j;
            
            j["origin"] = this->alert_key_.origin;
            j["type"] = this->alert_key_.type;
            j["subkey"] = this->alert_key_.subkey;
            
            j["priority"] = this->get_priority_string();
            j["severity"] = this->get_severity_string();
            j["message"] = this->alert_value_.message;
            
            return j.dump();    
        }
        
        int Deserialize(std::string str) {
            json j = json::parse(str);
            
            // special iterator member functions for objects
            for (json::iterator it = j.begin(); it != j.end(); ++it) {
                
                if (it.key() == "origin") {
                     this->alert_key_.origin = it.value();  
                } else if (it.key() == "type") {
                     this->alert_key_.type = it.value();  
                } else if (it.key() == "subkey") {
                     this->alert_key_.subkey = it.value();  
                } else if (it.key() == "priority") {
                    set_priority_from_string(it.value());
                } else if (it.key() == "severity") {
                    set_severity_from_string(it.value());
                } else if (it.key() == "message") {
                    this->alert_value_.message = it.value();
                }
                
            }
        }
        
        // Other
        
        // Make html table of data from alert, return string
        std::string MakeHTMLTable(){
            return 
            "<table style='width: 100%; border-collapse: collapse; margin-bottom: 20px' border=1 cellpadding=5>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Origin:</td>"
                    "<td>" + this->get_origin() + "</td>"
                "</tr>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Type:</td>"
                    "<td>" + this->get_type() + "</td>"
                "</tr>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Subkey:</td>"    
                    "<td>" + this->get_subkey() + "</td>"
                "</tr>" 
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Date and time:</td>"
                    "<td>" + this->get_creation_time_pretty() + "</td>"
                "</tr>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Priority:</td>"
                    "<td>" + this->get_priority_string() + "</td>"
                "</tr>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Severity:</td>"
                    "<td>" + this->get_severity_string() + "</td>"
                "</tr>"
                "<tr>"
                    "<td style='width:20%; text-align: right; font-weight: bold;'>Message:</td>"
                    "<td>" + this->get_message() + "</td>"
                "</tr>"
            "</table>";
        }
        
        // get time in good string
          
};