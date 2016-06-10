#pragma once

#include "Alert.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

struct InfoCell {
	std::vector<std::string> vec;
	std::string num;
	std::string origin;
	std::string type;
	std::string subkey;
	int AddCell(int num,std::string origin, std::string type, std::string subkey) {
		this->num = this->IntToString(num);
		this->origin = origin;
		this->type = type;
		this->subkey = subkey;
		return 0;
	}
	std::string IntToString(int value) {
		std::string result;
		std::ostringstream buf;
		buf << value;
		result = buf.str();
		return result;
	}
};

class Config {
private:
	json data_;
	std::string data_file_ = "/tmp/alertsubsystem/config.json";
public:
	Config() {
		std::string buf;
		buf = this->GetContents();
		this->data_ = json::parse(buf);
	}

	int ReadConfig() {
		std::string buf;
		buf = this->GetContents();
		this->data_ = json::parse(buf);
		return 0;
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

	Priority StrToPriority(std::string pr) {
		if (pr == "HIGH") {
			return HIGH;
		}
		if (pr == "MEDIUM") {
			return MEDIUM;
		}
		if (pr == "LOW") {
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
	
	Severity StrToSeverity(std::string sev) {
		if (sev == "CRITICAL") {
			return CRITICAL;
		}
		if (sev == "ERROR") {
			return ERROR;
		}
		if (sev == "WARNING") {
			return WARNING;
		}
		if (sev == "DEBUG") {
			return DEBUG;
		}
	}
	std::string PrintEmailVector() {
		std::vector<std::string> recipients_email = this->GetRecipientsEmail();
		std::string buf;
		for (std::vector<std::string>::iterator it = recipients_email.begin(); it != recipients_email.end(); ++it) {
			buf += (*it);
			buf += ',';
		}
		return buf;
	}
	std::string PrintEmailVector(std::string origin) {
		std::vector<std::string> recipients_email = this->GetRecipientsEmail(origin);
		std::string buf;
		for (std::vector<std::string>::iterator it = recipients_email.begin(); it != recipients_email.end(); ++it) {
			buf += (*it);
			buf += ',';
		}
		return buf;
	}
	std::string PrintEmailVector(std::string origin, std::string type) {
		std::vector<std::string> recipients_email = this->GetRecipientsEmail(origin, type);
		std::string buf;
		for (std::vector<std::string>::iterator it = recipients_email.begin(); it != recipients_email.end(); ++it) {
			buf += (*it);
			buf += ',';
		}
		return buf;
	}

	std::string PrintEmailVector(std::string origin, std::string type, std::string subkey) {
		std::vector<std::string> recipients_email = this->GetRecipientsEmail(origin,type,subkey);
		std::string buf;
		for (std::vector<std::string>::iterator it = recipients_email.begin(); it != recipients_email.end(); ++it) {
			buf += (*it);
			buf += ',';
		}
		return buf;
	}

	int MakeNewAlert(std::string origin,std::string type,std::string subkey) {	
		if (this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"].is_null()) {
			this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"] = "{}"_json;
		}
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int PrintDefinition(std::string origin) {		
		std::string buf = this->PrintEmailVector(origin);
		std::cout << "Message:" << this->GetMessage(origin) << std::endl;
		std::cout << "Time period of sending alerts:" << this->GetPeriodisityTime(origin) << std::endl;
		std::cout << "Priority:" << this->GetStrPriority(origin) << std::endl;
		std::cout << "Severity:" << this->GetStrSeverity(origin) << std::endl;
		std::cout << "Recipients email:" << buf << std::endl;		
		return 0;
	}

	int PrintDefinition(std::string origin, std::string type) {
		std::string buf = this->PrintEmailVector(origin, type);
		std::cout << "Message:" << this->GetMessage(origin, type) << std::endl;
		std::cout << "Time period of sending alerts:" << this->GetPeriodisityTime(origin, type) << std::endl;
		std::cout << "Priority:" << this->GetStrPriority(origin, type) << std::endl;
		std::cout << "Severity:" << this->GetStrSeverity(origin, type) << std::endl;
		std::cout << "Recipients email:" << buf << std::endl;
		return 0;
	}

	int PrintDefinition(std::string origin, std::string type, std::string subkey) {
		std::string buf = this->PrintEmailVector(origin, type,subkey);
		std::cout << "Message:" << this->GetMessage(origin, type,subkey) << std::endl;
		std::cout << "Time period of sending alerts:" << this->GetPeriodisityTime(origin, type,subkey) << std::endl;
		std::cout << "Priority:" << this->GetStrPriority(origin, type,subkey) << std::endl;
		std::cout << "Severity:" << this->GetStrSeverity(origin, type, subkey) << std::endl;
		std::cout << "Recipients email:" << buf << std::endl;
		return 0;
	}

	int PrintGlobalDefinition() {
		std::cout << "Mode:" << this->GetMode() << std::endl;
		std::string buf = this->PrintEmailVector();
		if (!this->Empty()) {
			std::cout << "Max alerts:" << this->GetMaxAlerts() << std::endl;
		}
		std::cout << "Message:" << this->GetMessage() << std::endl;
		std::cout << "Time period of sending alerts:" << this->GetPeriodisityTime() << std::endl;
		std::cout << "Priority:" << this->GetStrPriority() << std::endl;
		std::cout << "Severity:" << this->GetStrSeverity() << std::endl;
		std::cout << "Recipients email:" << buf << std::endl;
		return 0;
	}

	int WriteGlobalDefinition(std::string mode,std::string max_alerts,
							  std::string message,std::string periodicity_time,
							  std::string severity,std::string priority,std::vector<std::string> recipients_email) {
		this->SetMode(mode);
		this->SetMaxAlerts(std::stoi(max_alerts));
		this->SetMessage(message);
		this->SetPeriodisityTime(std::stol(periodicity_time));
		this->SetPriority(this->StrToPriority(priority));
		this->SetRecipientsEmail(recipients_email);
		return 0;
	}

	int WriteGlobalDefinition(std::string mode,	std::string message, std::string periodicity_time,
							  std::string severity, std::string priority, std::vector<std::string> recipients_email) {
		this->SetMode(mode);		
		this->SetMessage(message);
		this->SetPeriodisityTime(std::stol(periodicity_time));
		this->SetPriority(this->StrToPriority(priority));
		this->SetRecipientsEmail(recipients_email);
		return 0;
	}

	std::string GetRecepient() {
		std::string str = this->data_["definitions"]["defs"]["email"];
		return str;
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

	int SetPeriodisityTime(std::string origin, std::string type, std::string subkey, long periodicity_time) {
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["periodicity_time"] = periodicity_time;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime(std::string origin, std::string type, long periodicity_time) {
		this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["periodicity_time"] = periodicity_time;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime(std::string origin, long periodicity_time) {
		this->data_["definitions"]["origins"][origin]["defs"]["periodicity_time"] = periodicity_time;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetPeriodisityTime(long periodicity_time) {
		this->data_["definitions"]["defs"]["periodicity_time"] = periodicity_time;
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
		return this->CheckPriority(this->GetStrPriority(origin, type, subkey));
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
		json j;
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			j.push_back(*it);
		}
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"][subkey]["defs"]["recipients_email"] = j;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::string origin, std::string type, std::vector<std::string> vec) {
		json j;
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			j.push_back(*it);
		}
		this->data_["definitions"]["origins"][origin]["types"][type]["defs"]["recipients_email"] = j;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::string origin, std::vector<std::string> vec) {
		json j;
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			j.push_back(*it);
		}
		this->data_["definitions"]["origins"][origin]["defs"]["recipients_email"] = j;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	int SetRecipientsEmail(std::vector<std::string> vec) {
		json j;
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it) {
			j.push_back(*it);
		}
		this->data_["definitions"]["defs"]["recipients_email"] = j;
		this->WriteToDB(this->data_.dump());
		return 0;
	}

	std::string GetMode() {
		json j = this->data_["mode"];
		std::string str = j.dump();
		return str.substr(1, str.length() - 2);
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

	std::vector<InfoCell> ShowNotes() {		
		json j1 = this->data_["definitions"]["origins"];
		std::string buf = j1.dump();
		std::vector<InfoCell> info;
		json j = json::parse(buf);
		int num = 0;
		for (json::iterator it = j.begin(); it != j.end(); ++it) {			
			if (it.key() != "defs") {
				std::string origin = it.key();				
				json jtype = json::parse(it.value()["types"].dump());
				for (json::iterator il = jtype.begin(); il != jtype.end();++il) {
					if (il.key() != "defs") {
						std::string type = il.key();
						json jsubkey = json::parse(il.value()["subkeys"].dump());
						for (json::iterator iz = jsubkey.begin(); iz != jsubkey.end(); ++iz) {
							if (iz.key() != "defs") {
								std::string subkey = iz.key();
								InfoCell cell;
								cell.AddCell(num, origin, type, subkey);
								info.push_back(cell);
							}
							num++;
						}						
					}
				}
			}
		}
		return info;
	}

	int DeleteDefinition(std::string origin,std::string type,std::string subkey) {
		this->ReadConfig();
		json j1 = this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"];	
		std::string buf = j1.dump();		
		json j = json::parse(buf);
		json out;
		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			if (it.key() != subkey) {
				out += *it;
			}
		}
		this->data_["definitions"]["origins"][origin]["types"][type]["subkeys"] = out;
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

	int MakeLine(std::string num, std::string origin, std::string type, std::string subkey) {
		//for (int i = 0; i < 80; i++) {
		//	std::cout << '-';
		//}
		std::cout << "|" << std::setw(10) << num;
		std::cout << "|" << std::setw(27) << origin;
		std::cout << "|" << std::setw(27) << type;
		std::cout << "|" << std::setw(11) << subkey << "|";
		for (int i = 0; i < 80; i++) {
			std::cout << '-';
		}
		return 0;
	}

	int MakeTable() {
		std::string origin = "origin", type = "type", subkey = "subkey", num = "num";
		std::vector<InfoCell> info = this->ShowNotes();
		for (int i = 0; i < 80; i++) {
			std::cout << '-';
		}
		this->MakeLine(num,origin, type, subkey);
		for (std::vector<InfoCell>::iterator it = info.begin(); it != info.end(); ++it) {
			InfoCell buf = *it;
			this->MakeLine(buf.num, buf.origin, buf.type, buf.subkey);
		}
		return 0;
	}

	int Empty() {
		json j;
		j = json::parse(this->GetContents());
		if (j["mode"].is_null()) {
			return 1;
		}
		return 0;
	}
};
