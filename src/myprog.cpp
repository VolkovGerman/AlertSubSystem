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
    * GET Function
    */
    //std::cout << AlertSubSystem::GetAlert("./d.out", "face", "53").SerializeValue() << std::endl;

    /*
    * Priority sorting test
    */
    AlertSubSystem::PutAlert("./a.out", "math", "36", "bad", LOW, CRITICAL);
    AlertSubSystem::PutAlert("./d.out", "face", "13", "good", HIGH, CRITICAL);
    AlertSubSystem::PutAlert("./e.out", "vse", "34", "very_bad", LOW, CRITICAL);
    AlertSubSystem::PutAlert("./b.out", "tut", "76", "worse", LOW, CRITICAL);
    AlertSubSystem::PutAlert("./c.out", "vse", "34", "better", MEDIUM, CRITICAL);
    AlertSubSystem::PutAlert("./c1.out", "vse", "34", "much_better", MEDIUM, CRITICAL);
    AlertSubSystem::PutAlert("./r.out", "sobr", "99", "the_best", HIGH, CRITICAL);

    return 0;    
}