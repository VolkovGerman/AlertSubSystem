#include "alert.h"

namespace AlertSubSystem {
    
    std::vector<alert> get_alert(std::string origin);
    int put_alert();
    int sendToDaemon(std::string message);
    
}