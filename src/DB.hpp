#pragma once

#include <fstream>
#include <iostream>

#include "Alert.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

class DB {
    private:
        std::string data_file_ = "./db/all_data.json";      
    
    public:
        DB(){};
        
        std::string GetContents() {
            std::ifstream outfile;
            outfile.open(this->data_file_, std::ios::out | std::ios::app);
            
            std::string str;
            outfile >> str;
            
            outfile.close();
            
            return str;
        }
        
        int WriteToDB(std::string content) {
            std::ofstream outfile;
            outfile.open(this->data_file_, std::ios::out | std::ios::trunc);
            
            // write inputted data into the file.
            outfile << content << std::endl;
            
            outfile.close();
        }
        
        int Put(std::string str_alert) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);
            
            json jalert = json::parse(str_alert);

            json jAlertKey;
            jAlertKey["origin"] = jalert["origin"];
            jAlertKey["type"] = jalert["type"];
            jAlertKey["subkey"] = jalert["subkey"];

            if (!this->HasActiveAlert(jAlertKey.dump())) {
                jdata.push_back(jalert);
                this->WriteToDB(jdata.dump());
            }
            
            return 1;
        }
        
        std::string Get(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);
            
            
            json jfields = json::parse(str_fields);
            
            
            std::cout << jfields.dump(4) << std::endl;
            
            std::string origin, type, subkey;
            
            json result;
            
            if (jfields["origin"].is_null()) {
                // Return all objects in db
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    result.push_back(obj);
                }
                
            } else if (jfields["type"].is_null()) {
                // Return all objects with origin in db
                
                origin = jfields["origin"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] == origin){
                        result.push_back(obj);
                    } 
                }
                
            } else if (jfields["subkey"].is_null()) {
                // Return all objects with origin and type in db
                
                origin = jfields["origin"];
                type = jfields["type"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] == origin && obj["type"] == type){
                        result.push_back(obj);
                    } 
                }
                
            } else {
                // Return all objects with origin, type and subkey in db
                
                origin = jfields["origin"];
                type = jfields["type"];
                subkey = jfields["subkey"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] == origin && obj["type"] == type && obj["subkey"] == subkey){
                        result.push_back(obj);
                    } 
                }
                
            }
                
            return result.dump();
        }
        
        int Delete(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);
            
            json jfields = json::parse(str_fields);
            
            std::string origin, type;
            int subkey;
            
            json result;
            
            if (jfields["origin"].is_null()) {
                // Delete all objects in db
                
                result = "[]"_json;
                
            } else if (jfields["type"].is_null()) {
                // Delete all objects with origin in db
                
                origin = jfields["origin"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] != origin){
                        result.push_back(obj);
                    } 
                }
                
            } else if (jfields["subkey"].is_null()) {
                // Delete all objects with origin and type in db
                
                type = jfields["type"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] != origin || obj["type"] != type){
                        result.push_back(obj);
                    } 
                }
                
            } else {
                // Delete all objects with origin, type and subkey in db
                
                subkey = jfields["subkey"];
                
                for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                    json obj = *it;
                    if (obj["origin"] != origin || obj["type"] != type || obj["subkey"] != subkey){
                        result.push_back(obj);
                    } 
                }
            
            }
            
            this->WriteToDB(jdata.dump());
            return 1;   
        }

        int HasActiveAlert(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);

            json jfields = json::parse(str_fields);

            for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                json obj = *it;
                if (obj["origin"] == jfields["origin"] 
                    && obj["type"] == jfields["type"] 
                    && obj["subkey"] == jfields["subkey"]
                    && obj["state"] != "PROCESSED") {
                    return 1;
                } 
            }

            return 0;
        }

        int HasOldActiveAlert(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);

            json jfields = json::parse(str_fields);

            for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                json obj = *it;
                if (obj["origin"] == jfields["origin"] 
                    && obj["type"] == jfields["type"] 
                    && obj["subkey"] == jfields["subkey"]
                    && obj["state"] != "NEW"
                    && obj["state"] != "PROCESSED") {
                    return 1;
                } 
            }

            return 0;
        }

        int SetState(std::string str_fields, std::string new_state) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);

            json jfields = json::parse(str_fields);

            for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                json obj = *it;
                if (obj["origin"] == jfields["origin"] 
                    && obj["type"] == jfields["type"] 
                    && obj["subkey"] == jfields["subkey"]
                    && obj["state"] != "PROCESSED") {
                    (*it)["state"] = new_state;
                    break;
                } 
            }

            this->WriteToDB(jdata.dump());
            return 0;
        }

        int GetCreationTime(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);

            json jfields = json::parse(str_fields);

            for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                json obj = *it;
                if (obj["origin"] == jfields["origin"] 
                    && obj["type"] == jfields["type"] 
                    && obj["subkey"] == jfields["subkey"]
                    && obj["state"] != "PROCESSED") {
                    std::cout << obj["creation_time"] << std::endl;
                    auto num = obj["creation_time"];
                    return num;
                } 
            }

            return 0;
        }

        int SetCreationTime(std::string str_fields, int cr_time) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);

            json jfields = json::parse(str_fields);

            for (json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
                json obj = *it;
                if (obj["origin"] == jfields["origin"] 
                    && obj["type"] == jfields["type"] 
                    && obj["subkey"] == jfields["subkey"]
                    && obj["state"] != "PROCESSED") {
                    (*it)["creation_time"] = cr_time;
                    break;
                } 
            }

            this->WriteToDB(jdata.dump());
            return 0;
        }
    
};