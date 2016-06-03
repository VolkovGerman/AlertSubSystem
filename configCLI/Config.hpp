#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

enum Priority {
	HIGH,
	MEDIUM,
	LOW
};

enum Severity {
	CRITICAL,
	ERROR,
	WARNING,
	DEBUG
};

class Config {
private:
	json data_;
	std::string data_file_ = "config.json";
public:
	Config() {
		std::string buf;
		buf = this->GetContents();
		this->data_ = json::parse(buf);
	}

	Priority CheckPriority(std::string priority) {
		if (priority == "\"HIGH\"") {
			return HIGH;
		} 
		if (priority == "\"MEDIUM\"") {
			return MEDIUM;
		} 
		if (priority == "\"LOW\"") {
			return LOW;
		}

	}

	std::string PriorityToStr(Priority pr) {
		if (pr == HIGH) {
			return "HIGH";
		}
		if (pr == MEDIUM) {
			return "MEDIUM";
		}
		if (pr == LOW) {
			return "LOW";
		}
	}

	Severity CheckSeverity(std::string severity) {
		if (severity == "\"CRITICAL\"") {
			return CRITICAL;
		}
		if (severity == "\"ERROR\"") {
			return ERROR;
		}
		if (severity == "\"WARNING\"") {
			return WARNING;
		}
		if (severity == "\"DEBUG\"") {
			return DEBUG;
		}
	}

	std::string SeverityToStr(Severity sev) {
		if (sev == CRITICAL) {
			return "CRITICAL";
		}
		if (sev == ERROR) {
			return "ERROR";
		}
		if (sev == WARNING) {
			return "WARNING";
		}
		if (sev == DEBUG) {
			return "DEBUG";
		}
	}

	std::string GetMessage(std::string origin, std::string type, std::string subkey) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["message"];
		if (j.is_null()) {
			j = json::parse(this->GetMessage(origin, type));
		}
		return j.dump();
	}

	std::string GetMessage(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["message"];
		if (j.is_null()) {
			j = json::parse(this->GetMessage(origin));
		}
		return j.dump();
	}

	std::string GetMessage(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["message"];
		if (j.is_null()) {
			j = json::parse(this->GetMessage());
		}
		return j.dump();
	}

	std::string GetMessage() {
		json j = this->data_["definitions"]["defs"]["message"];
		std::string buf = j.dump();
		return buf;
	}

	int SetMessage(std::string origin, std::string type, std::string subkey, std::string message) {
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["message"] = message;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetMessage(std::string origin, std::string type, std::string message) {
		this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["message"] = message;		
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetMessage(std::string origin, std::string message) {
		this->data_["definitions"]["origins"][origin]["defs"]["message"] = message;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetMessage(std::string message) {
		this->data_["definitions"]["defs"]["message"] = message;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	long GetPeriodisityTime(std::string origin, std::string type, std::string subkey) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["periodicity_time"];		
		if (j.is_null()) {
			j = this->GetPeriodisityTime(origin, type);
		}
		return j;
	}

	long GetPeriodisityTime(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["periodicity_time"];
		if (j.is_null()) {
			j = this->GetPeriodisityTime(origin);
		}
		return j;
	}
	
	long GetPeriodisityTime(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["periodicity_time"];
		if (j.is_null()) {
			j = this->GetPeriodisityTime();
		}
		return j;
	}

	long GetPeriodisityTime() {
		json j = this->data_["definitions"]["defs"]["periodicity_time"];		
		return j;
	}

	int SetPeriodisityTime(std::string origin, std::string type, std::string subkey, long time) {
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["periodicity_time"] = time;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["periodicity_time"];
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["periodicity_time"];
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime() {
		json j = this->data_["definitions"]["defs"]["periodicity_time"];
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	std::string GetStrPriority(std::string origin, std::string type, std::string subkey) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["priority"];
		if (j.is_null()) {
			j = json::parse(this->GetStrPriority(origin, type));
		}
		return j.dump();
	}

	std::string GetStrPriority(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["priority"];
		if (j.is_null()) {
			j = json::parse(this->GetStrPriority(origin));
		}
		return j.dump();
	}

	std::string GetStrPriority(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["priority"];
		if (j.is_null()) {
			j = json::parse(this->GetStrPriority());
		}
		return j.dump();
	}

	std::string GetStrPriority() {
		json j = this->data_["definitions"]["defs"]["priority"];		
		return j.dump();
	}

	Priority GetPriority(std::string origin, std::string type, std::string subkey) {
		return this->CheckPriority(this->GetStrPriority(origin,type, subkey));
	}

	Priority GetPriority(std::string origin, std::string type) {
		return this->CheckPriority(this->GetStrPriority(origin, type));
	}

	Priority GetPriority(std::string origin) {
		return this->CheckPriority(this->GetStrPriority(origin));
	}

	Priority GetPriority() {
		return this->CheckPriority(this->GetStrPriority());
	}

	int SetPriority(std::string origin, std::string type, std::string subkey, Priority pr) {
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["priority"] = this->PriorityToStr(pr);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPriority(std::string origin, std::string type, Priority pr) {
		this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["priority"] = this->PriorityToStr(pr);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPriority(std::string origin, Priority pr) {
		this->data_["definitions"]["origins"][origin]["defs"]["priority"] = this->PriorityToStr(pr);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPriority(Priority pr) {
		this->data_["definitions"]["defs"]["priority"] = this->PriorityToStr(pr);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	std::string GetStrSeverity(std::string origin, std::string type, std::string subkey) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["severity"];
		if (j.is_null()) {
			j = json::parse(this->GetStrSeverity(origin, type));
		}
		return j.dump();
	}

	std::string GetStrSeverity(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["severity"];
		if (j.is_null()) {
			j = json::parse(this->GetStrSeverity(origin));
		}
		return j.dump();
	}

	std::string GetStrSeverity(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["severity"];
		if (j.is_null()) {
			j = json::parse(this->GetStrSeverity());
		}
		return j.dump();
	}

	std::string GetStrSeverity() {
		json j = this->data_["definitions"]["defs"]["severity"];
		return j.dump();
	}


	Severity GetSeverity(std::string origin, std::string type, std::string subkey) {
		return this->CheckSeverity(this->GetStrSeverity(origin, type, subkey));
	}

	Severity GetSeverity(std::string origin, std::string type) {
		return this->CheckSeverity(this->GetStrSeverity(origin, type));
	}

	Severity GetSeverity(std::string origin) {
		return this->CheckSeverity(this->GetStrSeverity(origin));
	}

	Severity GetSeverity() {
		return this->CheckSeverity(this->GetStrSeverity());
	}

	int SetSeverity(std::string origin, std::string type, std::string subkey, Severity sev) {
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["severity"] = this->SeverityToStr(sev);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetSeverity(std::string origin, std::string type, Severity sev) {
		this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["severity"] = this->SeverityToStr(sev);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetSeverity(std::string origin, Severity sev) {
		this->data_["definitions"]["origins"][origin]["defs"]["severity"] = this->SeverityToStr(sev);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetSeverity(Severity sev) {
		this->data_["definitions"]["defs"]["severity"] = this->SeverityToStr(sev);
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type, std::string subkey) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["recipients_email"];		
		if (j.is_null()) {
			std::vector<std::string> buf = this->GetRecipientsEmail(origin, type);
			return buf;
		}
		std::vector<std::string> vec = j;
		return vec;
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin, std::string type) {
		json j = this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["recipients_email"];		
		if (j.is_null()) {
			std::vector<std::string> buf = this->GetRecipientsEmail(origin);
			return buf;
		}
		std::vector<std::string> vec = j;
		return vec;
	}

	std::vector<std::string> GetRecipientsEmail(std::string origin) {
		json j = this->data_["definitions"]["origins"][origin]["defs"]["recipients_email"];			
		if (j.is_null()) {
			std::vector<std::string> buf = this->GetRecipientsEmail();
			return buf;
		} 	
		std::vector<std::string> vec = j;
		return vec;
	}

	std::vector<std::string> GetRecipientsEmail() {
		json j = this->data_["definitions"]["defs"]["recipients_email"];		
		std::vector<std::string> vec = j;			
		return vec;
	}	

	int SetRecipientsEmail(std::string origin, std::string type, std::string subkey, std::vector<std::string> vec) {
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["recipients_email"].push_back(*it);
		}
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::string origin, std::string type, std::vector<std::string> vec) {
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["recipients_email"].push_back(*it);
		}
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::string origin, std::vector<std::string> vec) {
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			this->data_["definitions"]["origins"][origin]["defs"]["recipients_email"].push_back(*it);
		}
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::vector<std::string> vec) {
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			this->data_["definitions"]["defs"]["recipients_email"].push_back(*it);
		}
		this->WriteToDB(this->data_.dump());
		return 0;
	}
	
	std::string GetMode() {
		json j = this->data_["mode"];		
		return j.dump();
	}

	int SetMode(std::string mode) {
		this->data_["mode"] = mode;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int GetMaxAlerts() {
		json j = this->data_["max_alerts"];
		return j;
	}

	int SetMaxAlerts(int max_alert) {
		this->data_["max_alerts"] = max_alert;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int AddDefinition() {
		return 0;
	}

	std::string GetContents() {
		std::ifstream outfile;
		outfile.open(this->data_file_, std::ios::out | std::ios::app);

		std::string str;
		outfile >> str;		
		outfile.close();

		return str;
	}

	int WriteToDB(std::string content) {
		std::ofstream outfile;
		outfile.open(this->data_file_, std::ios::out | std::ios::trunc);

		// write inputted data into the file.
		outfile << content;

		outfile.close();
		return 0;
	}
};
