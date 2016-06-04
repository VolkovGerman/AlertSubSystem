#include <iostream>
#include "ASInterface.hpp"

int main(){ 
    
    AlertSubSystem::PutAlert("./b.out", "math", "53");
    
    Alert alert1 = AlertSubSystem::GetAlert("./b.out", "math", "53");
    std::cout << alert1.Serialize() << std::endl;

    return 0;    
}