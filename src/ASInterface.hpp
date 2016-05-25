#include "Alert.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

namespace AlertSubSystem{
    
    int put(std::string origin, std::string type, int subkey) {
        
        Alert oldAlert(origin, type, subkey);
        
        std::string str1 = oldAlert.serialize();
        std::cout << str1;
        
        Alert newAlert;
        newAlert.deserialize(str1);
        
        std::cout << newAlert.get_origin() << ":" << newAlert.get_type() << ":" << newAlert.get_subkey() << std::endl;
        
        std::cout << " " << std::endl;
        
        json j = "{\"0\": {\"origin\":\"./a.out\"}, \"1\": {\"origin\":\"./a.out\"}}"_json;
    for (json::iterator it = j.begin(); it != j.end(); ++it) {        

            std::cout << it.key() << " ";  

    }
    }
    
}