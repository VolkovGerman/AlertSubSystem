#include <iostream>
#include "ASInterface.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

int main(){
    
    //AlertSubSystem::PutAlert("./a.out", "math", 55);  
    
    AlertSubSystem::PutAlert("./B.out", "math", 53);
    //std::cout << str << std::endl;
    //AlertSubSystem::Alert newAl = AlertSubSystem::GetAlert("./a.out", "math", 53);
    
    /*
    json j, j1;
    j1["origin"] = 1;
    j1["type"] = 2;
    j.push_back(j1.dump());
    j.push_back(j1.dump());
    j.push_back(j1.dump());
    
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::string j1 = *it;
        json j2 = json::parse(j1);
        
        for (json::iterator it1 = j2.begin(); it1 != j2.end(); ++it1) {
            std::cout << it1.key() << " --- " << it1.value() << "\n";
        }
    }
    */
    
    return 0;    
}