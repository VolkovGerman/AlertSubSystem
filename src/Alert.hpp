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
	LOW,
    NO_PR
};

enum Severity {
    CRITICAL,
	ERROR,  
	WARNING,
	DEBUG,
    NO_SV
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
        Alert(){
            alert_value_.creation_time = time(0);
            alert_value_.priority = NO_PR;
            alert_value_.severity = NO_SV;
            alert_value_.message = "";
        };
        
        Alert(std::string origin, std::string type, std::string subkey) {
            alert_key_.origin = origin;
            alert_key_.type = type;
            alert_key_.subkey = subkey;

            alert_value_.creation_time = time(0);
            alert_value_.priority = NO_PR;
            alert_value_.severity = NO_SV;
            alert_value_.message = "";
        }
        
        Alert(std::string origin, std::string type) {
            alert_key_.origin = origin;
            alert_key_.type = type;

            alert_value_.creation_time = time(0);
            alert_value_.priority = NO_PR;
            alert_value_.severity = NO_SV;
            alert_value_.message = "";
        }
        
        Alert(std::string origin) {
            alert_key_.origin = origin;

            alert_value_.creation_time = time(0);
            alert_value_.priority = NO_PR;
            alert_value_.severity = NO_SV;
            alert_value_.message = "";
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
            return this->alert_value_.priority;
        }
        
        std::string get_priority_string(){
            switch (alert_value_.priority) {
                case HIGH:
                    return "HIGH";
                case MEDIUM:
                    return "MEDIUM";
                case LOW:
                    return "LOW";
                case NO_PR:
                    return "NO_PRIORITY";
                default:
                    return "";
            }
            
            return "";
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
            } else if (pr == "NO_PR") {
                this->alert_value_.priority = NO_PR;
            }
            
            return 0;
        }
        
        Severity get_severity(){
            return alert_value_.severity;
        }
        
        std::string get_severity_string(){
            switch (alert_value_.severity) {
                case ERROR:
                    return "ERROR";
                case CRITICAL:
                    return "CRITICAL";
                case WARNING:
                    return "WARNING";
                case DEBUG:
                    return "DEBUG";
                case NO_SV:
                    return "NO_SV";
                default:
                    return "";
            }
            
            return "";
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
            } else if (sv == "NO_SV") {
                this->alert_value_.severity = NO_SV;
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

        int set_creation_time() {
            time_t now = time(0);
            std::cout << "При создании" << now << std::endl;
            this->alert_value_.creation_time = now;
            return 0;  
        }
        
        time_t get_creation_time() {
            return this->alert_value_.creation_time;
        }
        
        std::string get_creation_time_pretty() {  
            tm *ltm = localtime(&(this->alert_value_.creation_time));
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

        bool operator <(Alert & alertB) {
            Priority prA = this->get_priority();
            Priority prB = alertB.get_priority();

            int numA, numB;
            switch (prA) {
                case HIGH:
                    numA = 3;
                    break;
                case MEDIUM:
                    numA = 2;
                    break;
                case LOW:
                    numA = 1;
                    break;
                case NO_PR:
                    numA = 0;
                    break;
            }

            switch (prB) {
                case HIGH:
                    numB = 3;
                    break;
                case MEDIUM:
                    numB = 2;
                    break;
                case LOW:
                    numB = 1;
                    break;
                case NO_PR:
                    numB = 0;
                    break;
            }

            std::cout << "COOOOMPAAAAAAAAAAAARING" << std::endl;

            std::cout << numA << " ----- " << numB << std::endl;

            std::cout << "//COOOOMPAAAAAAAAAAAARING" << std::endl;

            return (numA - numB >= 0) ? true : false;
        }
        
        // Serialize / Deserialize
        
        std::string SerializeKey() {
            json j;
            
            j["origin"] = this->alert_key_.origin;
            j["type"] = this->alert_key_.type;
            j["subkey"] = this->alert_key_.subkey;
            
            return j.dump();
        }
        
        std::string SerializeValue() {
            json j;
            
            j["message"] = this->alert_value_.message;
            j["severity"] = this->get_severity_string();
            j["priority"] = this->get_priority_string();
            
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
                } else if (it.key() == "creation_time") {
                    this->alert_value_.creation_time = it.value();
                }
                
            }
            
            return 0;
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