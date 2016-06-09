#include <iostream>
#include "ASInterface.hpp"

int main(){ 
    /*
    * PUT Functions
    */

    //AlertSubSystem::PutAlert("./d.out", "face", "53");

    //AlertSubSystem::PutAlert("./d.out", "face", "53", "runawaay!!www");
    //AlertSubSystem::PutAlert("./d.out", "face", "53", WARNING);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", MEDIUM);

    //AlertSubSystem::PutAlert("./d.out", "face", "53", "runawaay!!w1w1w", MEDIUM);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", "runawaay!!w2w2w", DEBUG);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", HIGH, "runawaay!!w3w3w");
    //AlertSubSystem::PutAlert("./d.out", "face", "53", MEDIUM, DEBUG);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", DEBUG, "runawaay!!w4w4w");
    //AlertSubSystem::PutAlert("./d.out", "face", "53", CRITICAL, LOW);

    //AlertSubSystem::PutAlert("./d.out", "face", "53", "runawaay!!w5w5w", CRITICAL, LOW);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", "runawaay!!w1w1w", LOW, CRITICAL);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", CRITICAL, "runawaay!!w1w1w", LOW);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", CRITICAL, LOW, "runawaay!!w1w1w");
    //AlertSubSystem::PutAlert("./d.out", "face", "53", LOW, "runawaay!!w1w1w", CRITICAL);
    //AlertSubSystem::PutAlert("./d.out", "face", "53", HIGH, DEBUG, "runawaay!!w1w1w");


    /*
    * GET Functions
    */
    std::cout << AlertSubSystem::GetAlert("./d.out", "face", "53").SerializeValue() << std::endl;

    return 0;    
}