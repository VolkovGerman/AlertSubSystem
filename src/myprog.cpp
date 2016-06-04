#include <iostream>
#include "ASInterface.hpp"

int main(){ 
    AlertSubSystem::PutAlert("./d.out", "face", "53");
    
    //Alert alert1 = AlertSubSystem::GetAlert("./d.out", "face", "53");
    //std::cout << alert1.SerializeValue() << std::endl;

    return 0;    
}