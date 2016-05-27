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
            
            jdata.push_back(jalert);
            
            this->WriteToDB(jdata.dump());
            return 1;
        }
        
        std::string Get(std::string str_fields) {
            std::string file_data = this->GetContents();
            json jdata = json::parse(file_data);
            
            json jfields = json::parse(str_fields);
            
            std::string origin, type;
            int subkey;
            
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
    
};