#include <iostream>

#include "DB.hpp"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

int main() {

    DB database;

    std::string str = database.GetContents();

    json jArr = json::parse(str);
    json result;

    std::cout << "AlertSubsystem Database Contents" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (json::iterator it = jArr.begin(); it != jArr.end(); ++it) {
        json jObj = *it;
    
        for (json::iterator it = jObj.begin(); it != jObj.end(); ++it) {
            std::cout << "|" << jObj["origin"] << "|" << jObj["type"] << "|" << jObj["subkey"] << "|";

            if (!jObj["priority"].is_null()) {
                std::cout << jObj["priority"];
            } else {
                std::cout << "ZZZZ";
            }

            if (!jObj["severity"].is_null()) {
                std::cout << jObj["severity"];
            } else {
                std::cout << "ZZZZ";
            }

            if (!jObj["message"].is_null()) {
                std::cout << jObj["message"];
            } else {
                std::cout << "ZZZZ";
            }

            std::cout << std::endl;
        }
    
    }

    return 0;
}