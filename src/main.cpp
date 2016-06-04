#include "Config.hpp"
#include <stdio.h>
//#include "interface.cpp"






int CheckNumber(std::string number) {
	for (int i = 0; i < number.size(); i++) {
		if (number[i]<'0' || number[i]>'9') {
			return 0;
		}
	}
	return 1;
}

int CheckSpaces(std::string number) {
	for (int i = 0; i < number.size(); i++) {
		if (number[i]==' ') {
			return 0;
		}
	}
	return 1;
}

std::vector<std::string> EmailToVec(std::string email) {
	std::vector<std::string> recipients_email;
	std::stringstream ff;
	std::string buf;
	ff << email;
	while (getline(ff, buf, ',')) {
		recipients_email.push_back(buf);
	}
	return recipients_email;
}

int EnterDefinition(std::string origin,std::string type,std::string subkey) {
	std::vector<std::string> email_vec;
	std::string mode, max_alerts, periodicity_time, message, priority, severity, recipients_email;	
	Config obj;	
	bool flag;
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter message:";
		flag = true;
		std::getline(std::cin, message);
		if (message.empty()) {
			flag = false;
			break;			
		}
		if (!CheckSpaces(message)) {
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			flag = false;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetMessage(origin, type, subkey, message);		
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter time period of sending alerts:";
		flag = true;
		std::getline(std::cin, periodicity_time);
		if (periodicity_time.empty()) {
			flag = false;
			break;
		}
		if (!CheckNumber(periodicity_time)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPeriodisityTime(origin, type, subkey, std::stol(periodicity_time));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Priority: HIGH, MEDIUM, LOW" << std::endl;
		std::cout << "Enter priority:";
		flag = true;
		std::getline(std::cin, priority);
		if (priority.empty()) {
			flag = false;
			break;
		}
		if (priority != "HIGH" && priority != "MEDIUM" && priority != "LOW") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPriority(origin, type, subkey, obj.StrToPriority(priority));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Severity: CRITICAL, ERROR, WARNING, DEBUG" << std::endl;
		std::cout << "Enter severity:";
		flag = true;
		std::getline(std::cin, severity);
		if (severity.empty()) {
			flag = false;
			break;
		}
		if (severity != "CRITICAL" && severity != "ERROR" && severity != "WARNING" && severity != "DEBUG") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
		if (flag == true) {
			obj.SetSeverity(origin, type, subkey, obj.StrToSeverity(severity));
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter recipients email (you can record several emails,seperated by commas):" << std::endl;
		std::cout << "Example of two emails:bylnov.nick@gmail.com,Volkov.german.1997@gmail.com" << std::endl;
		flag = true;
		std::getline(std::cin, recipients_email);
		if (recipients_email.empty()) {
			flag = false;
			break;
		}
		if (!CheckSpaces(recipients_email)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		email_vec = EmailToVec(recipients_email);
		obj.SetRecipientsEmail(origin, type, subkey, email_vec);
	}
	return 0;
}

int EnterDefinition(std::string origin, std::string type) {
	std::vector<std::string> email_vec;
	std::string mode, max_alerts, periodicity_time, message, priority, severity, recipients_email;
	Config obj;
	bool flag;
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter message:";
		flag = true;
		std::getline(std::cin, message);
		if (message.empty()) {
			flag = false;
			break;
		}
		if (!CheckSpaces(message)) {
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			flag = false;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetMessage(origin, type, message);
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter time period of sending alerts:";
		flag = true;
		std::getline(std::cin, periodicity_time);
		if (periodicity_time.empty()) {
			flag = false;
			break;
		}
		if (!CheckNumber(periodicity_time)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPeriodisityTime(origin, type, std::stol(periodicity_time));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Priority: HIGH, MEDIUM, LOW" << std::endl;
		std::cout << "Enter priority:";
		flag = true;
		std::getline(std::cin, priority);
		if (priority.empty()) {
			flag = false;
			break;
		}
		if (priority != "HIGH" && priority != "MEDIUM" && priority != "LOW") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPriority(origin, type, obj.StrToPriority(priority));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Severity: CRITICAL, ERROR, WARNING, DEBUG" << std::endl;
		std::cout << "Enter severity:";
		flag = true;
		std::getline(std::cin, severity);
		if (severity.empty()) {
			flag = false;
			break;
		}
		if (severity != "CRITICAL" && severity != "ERROR" && severity != "WARNING" && severity != "DEBUG") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
		if (flag == true) {
			obj.SetSeverity(origin, type, obj.StrToSeverity(severity));
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter recipients email (you can record several emails,seperated by commas):" << std::endl;
		std::cout << "Example of two emails:bylnov.nick@gmail.com,Volkov.german.1997@gmail.com" << std::endl;
		flag = true;
		std::getline(std::cin, recipients_email);
		if (recipients_email.empty()) {
			flag = false;
			break;
		}
		if (!CheckSpaces(recipients_email)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		email_vec = EmailToVec(recipients_email);
		obj.SetRecipientsEmail(origin, type, email_vec);
	}
	return 0;
}

int EnterDefinition(std::string origin) {
	std::vector<std::string> email_vec;
	std::string mode, max_alerts, periodicity_time, message, priority, severity, recipients_email;
	Config obj;
	bool flag;
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter message:";
		flag = true;
		std::getline(std::cin, message);
		if (message.empty()) {
			flag = false;
			break;
		}
		if (!CheckSpaces(message)) {
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			flag = false;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {	
		obj.SetMessage(origin, message);
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter time period of sending alerts:";
		flag = true;
		std::getline(std::cin, periodicity_time);
		if (periodicity_time.empty()) {
			flag = false;
			break;
		}
		if (!CheckNumber(periodicity_time)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPeriodisityTime(origin, std::stol(periodicity_time));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Priority: HIGH, MEDIUM, LOW" << std::endl;
		std::cout << "Enter priority:";
		flag = true;
		std::getline(std::cin, priority);
		if (priority.empty()) {
			flag = false;
			break;
		}
		if (priority != "HIGH" && priority != "MEDIUM" && priority != "LOW") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		obj.SetPriority(origin, obj.StrToPriority(priority));
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Severity: CRITICAL, ERROR, WARNING, DEBUG" << std::endl;
		std::cout << "Enter severity:";
		flag = true;
		std::getline(std::cin, severity);
		if (severity.empty()) {
			flag = false;
			break;
		}
		if (severity != "CRITICAL" && severity != "ERROR" && severity != "WARNING" && severity != "DEBUG") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
		if (flag == true) {
			obj.SetSeverity(origin, obj.StrToSeverity(severity));
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter recipients email (you can record several emails,seperated by commas):" << std::endl;
		std::cout << "Example of two emails:bylnov.nick@gmail.com,Volkov.german.1997@gmail.com" << std::endl;
		flag = true;
		std::getline(std::cin, recipients_email);
		if (recipients_email.empty()) {
			flag = false;
			break;
		}
		if (!CheckSpaces(recipients_email)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	if (flag == true) {
		email_vec = EmailToVec(recipients_email);
		obj.SetRecipientsEmail(origin, email_vec);
	}
	return 0;
}


int EnterGlobalDefintion() {
	std::vector<std::string> email_vec;
	std::string mode, max_alerts,periodicity_time,message,priority,severity, recipients_email;
	Config obj;
	bool flag = false;
	while (flag == false) {
		std::cout << "modes:immidiatly,batch" << std::endl;
		std::cout << "Enter mode:";
		flag = true;
		std::getline(std::cin, mode);
		if (mode != "immidiatly" && mode!="batch" || mode.empty()) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin); 
	std::system("clear");
	flag = false;
	if (mode == "batch") {
		while (flag == false) {
			std::cout << "Enter number of alerts in one batch:";
			flag = true;
			std::getline(std::cin, max_alerts);
			if (!CheckNumber(max_alerts) || max_alerts.empty()) {
				flag = false;
				std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
				getchar();
				std::system("clear");
			}
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter time period of sending alerts:";
		flag = true;
		std::getline(std::cin, periodicity_time);
		if (!CheckNumber(periodicity_time) || periodicity_time.empty()) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter message:";
		flag = true;
		std::getline(std::cin, message);
		if (!CheckSpaces(message) ||message.empty()) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Priority: HIGH, MEDIUM, LOW" << std::endl;
		std::cout << "Enter priority:";
		flag = true;
		std::getline(std::cin, priority);
		if (priority.empty() || priority!="HIGH" && priority!="MEDIUM" && priority!="LOW") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}	
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Severity: CRITICAL, ERROR, WARNING, DEBUG" << std::endl;
		std::cout << "Enter severity:";
		flag = true;
		std::getline(std::cin, severity);
		if (severity.empty() || severity != "CRITICAL" && severity != "ERROR" && severity != "WARNING" && severity != "DEBUG") {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter recipients email (you can record several emails,seperated by commas):" << std::endl;
		std::cout << "Example of two emails:bylnov.nick@gmail.com,Volkov.german.1997@gmail.com" << std::endl;		
		flag = true;
		std::getline(std::cin, recipients_email);
		if (recipients_email.empty() || !CheckSpaces(recipients_email)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	email_vec = EmailToVec(recipients_email);
	if (mode == "immidiatly") {
		obj.WriteGlobalDefinition(mode, message, periodicity_time, severity, priority, email_vec);
	} else {
		obj.WriteGlobalDefinition(mode,max_alerts, message, periodicity_time, severity, priority, email_vec);
	}
	return 0;
}

int MakeAlert() {
	Config obj;
	std::string origin, type, subkey;
	bool flag = false;
	while (flag == false) {
		std::cout << "Enter origin:";
		flag = true;
		std::getline(std::cin, origin);
		if (origin.empty() || !CheckSpaces(origin)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter type:";
		flag = true;
		std::getline(std::cin, type);
		if (type.empty() || !CheckSpaces(type)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	fflush(stdin);
	std::system("clear");
	flag = false;
	while (flag == false) {
		std::cout << "Enter subkey:";
		flag = true;
		std::getline(std::cin, subkey);
		if (subkey.empty() || !CheckNumber(subkey)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	obj.MakeNewAlert(origin, type, subkey);
	return 0;
}

int FulfillCell(std::vector<InfoCell> info) {
	bool flag = false;
	std::string number;
	while (flag == false) {
		std::cout << "Enter number of alert:";
		flag = true;
		std::getline(std::cin, number);
		if (number.empty()) {
			getchar();
			std::system("clear");
			return -1;
		}
		if (!CheckNumber(number)) {
			flag = false;
			std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
			getchar();
			std::system("clear");
		}
	}
	int num = std::stoi(number);
	if (num < 0 || num > info.size() - 1) {
		std::cout << "Error.Out of range";
		return -1;
	}
	return num;
}

int AlertTable() {
	int flag;
	Config obj;
	std::string choice;
	while (1) {
		std::system("clear");
		flag = false;
		while (flag == false) {
			std::cout << " Alert table " << std::endl;
			std::cout << "1.Show alert table" << std::endl;
			std::cout << "2.Make new alert" << std::endl;
			std::cout << "3.Set/Change alert information" << std::endl;
			std::cout << "4.Set/Change type define information" << std::endl;
			std::cout << "5.Set/Change origin define information" << std::endl;
			std::cout << "6.Show alert information" << std::endl;
			std::cout << "7.Show type define information" << std::endl;
			std::cout << "8.Show origin define information" << std::endl;
			std::cout << "9.Delete alert information" << std::endl;
			std::cout << "10.Back to menu" << std::endl;		
			flag = true;
			std::getline(std::cin, choice);
			if (!CheckNumber(choice) || choice.empty()) {
				flag = false;
				std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
				getchar();
				std::system("clear");
			}
		}
		switch (std::stoi(choice)) {
		case 1: {			
			std::system("clear");
			obj.ReadConfig();
			obj.MakeTable();
			getchar();
			break;
		}
		case 2: {
			std::system("clear");
			MakeAlert();
			getchar();
			break;
		}
		case 3: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}			
			EnterDefinition((*it).origin,(*it).type,(*it).subkey);
			break;
		}
		case 4: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			EnterDefinition((*it).origin, (*it).type);
			getchar();
			break;

		}
		case 5: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			EnterDefinition((*it).origin);
			getchar();
			break;
		}
		case 6: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			obj.PrintDefinition((*it).origin,(*it).type,(*it).subkey);
			getchar();
			break;
		}
		case 7: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			obj.PrintDefinition((*it).origin, (*it).type);
			getchar();
			break;
		}
		case 8: {
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			obj.PrintDefinition((*it).origin);
			getchar();
			break;
		}
		case 9:{
			std::system("clear");
			obj.ReadConfig();
			std::vector<InfoCell> info = obj.ShowNotes();
			if (info.empty()) {
				std::cout << "Error.Empty alert table";
				getchar();
				break;
			}
			int num = FulfillCell(info);
			if (num == -1) {
				break;
			}
			std::vector<InfoCell>::iterator it = info.begin();
			for (int i = 0; i < num; i++) {
				it++;
			}
			obj.DeleteDefinition((*it).origin,(*it).type,(*it).subkey);
			getchar();
			break;			
		}
		case 10: {
			return 0;
		}				
		}
	}
	return 0;
}



/*
int mainhead() {
Config data;
//if (data.Empty()) {
//std::cout << "Your config is empty,enter global definitions" << std::endl;
EnterGlobalDefintion();
//}
return 0;
}
*/


int main() {
	Config obj;	
	int flag;
	std::string choice;
	if (obj.Empty()) {
		EnterGlobalDefintion();
	}
	while (1) {	
		std::system("clear");
		flag = false;
		while (flag == false) {
			std::cout << " Config file " << std::endl;		
			std::cout << "1.Show global definitions" << std::endl;
			std::cout << "2.Change global definitions" << std::endl;
			std::cout << "3.Go to alert table" << std::endl;
			std::cout << "4.Exit" << std::endl;			
			flag = true;
			std::getline(std::cin, choice);
			if (!CheckNumber(choice) || choice.empty()) {
				flag = false;
				std::cout << "Error.Wrong name, enter this parameter again" << std::endl;
				getchar();
				std::system("clear");
			}
		}
		switch (std::stoi(choice))	{
		case 1: { 
			obj.ReadConfig();
			obj.PrintGlobalDefinition();
			getchar();
			break;
		}
		case 2: { 
			EnterGlobalDefintion();
			getchar();
			break;
		}
		case 3: { 
			AlertTable();
			break;
		}
		case 4:return 0;
		default:break;
		}
	}
	return 0;
}
