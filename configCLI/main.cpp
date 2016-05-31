#include "Config.hpp"


int fun() {
	json j;
	Config obj;
	std::string buf = obj.GetContents();
	j = json::parse(buf);
	std::string origin, type, subkey;
	std::cout << "Enter origin: ";
	std::cin >> origin;
	std::cout << "Enter type: ";
	std::cin >> type;
	std::cout << "Enter subkey: ";
	std::cin >> subkey;
	j["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"] = "{}"_json;
	obj.WriteToDB(j.dump());
	return 0;
}


int main() {
	Config obj;	
	//std::cout << obj.GetMessage("./a.out","math","52");
	json j;	
	std::string buf = obj.GetContents();
	j = json::parse(buf);
	//std::cout << j.dump(4);
	/*
	if (j["mode"].is_null()) {	
		std::vector<std::string> vec;
		vec.push_back("bylnov.nick@yandex.ru");
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			j["definitions"]["defs"]["recipients_email"].push_back(*it);
		}
		std::cout << "Enter mode: ";
		std::string str,gl_mes;
		int pr_time;
		std::cin >> str;
		j["mode"] = str;
		j["max_alerts"] = 10;
		std::cout << "Enter global def_message:";
		std::cin >> gl_mes;
		j["definitions"]["defs"]["message"] = gl_mes;
		std::cout << "Enter global def_periodicity_time:";
		std::cin >> pr_time;
		j["definitions"]["defs"]["periodicity_time"] = pr_time;
		j["definitions"]["defs"]["priority"] = "LOW";
		j["definitions"]["defs"]["severity"] = "ERROR";
		obj.WriteToDB(j.dump());
	}
	else {
		fun();
	}
	*/	
	
	
	return 0;
}