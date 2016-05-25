#include <iostream>
#include "ASInterface.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

int main(){
    
    AlertSubSystem::put("./a.out", "math", 53);  
    
    
    
    return 0;    
}