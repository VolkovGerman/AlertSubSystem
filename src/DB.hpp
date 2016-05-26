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
        
        int Put(std::string str_alert) {
            std::string file_data = this->GetContents();
           
            json jdata = json::parse(file_data);
            json jalert = json::parse(str_alert);
            
            jdata.push_back(jalert);
            
            std::ofstream outfile;
            outfile.open(this->data_file_, std::ios::out | std::ios::trunc);
            
            // write inputted data into the file.
            outfile << jdata.dump() << std::endl;
            
            outfile.close();
            return 1;
        }
    
};