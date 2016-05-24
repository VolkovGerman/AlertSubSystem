#pragma once

#include <iostream>
#include <vector>

enum PRIORITY {
	HIGH,
	MEDIUM,
	LOW
};
enum SEVERITY {
	CRITICAL,
	ERROR,
	WARNING,
	DEBUG
};

struct date {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

struct timelimit {
	int lday;
	int lhour;
	int lminute;
	int lsecond;
};

struct key {
	std::string origin;
	std::string type;
	int subkey;
};

struct value {	
	date curTime;
	timelimit limTime;
	std::string sysTime;
	std::string message;
	std::string state;
	PRIORITY pr;
	SEVERITY sev;
};

class alert {
private:
	std::vector<std::string> prior;
	std::vector<std::string> sever;		
	key k;
	value node;
public:
	alert();
	std::string serialize(char n);
	int initFromString(std::string info);
	std::string IntToString(int value);
	std::string GetLocalTime();
	std::string AllTime();
	std::string getTime();
	friend std::fstream & operator >> (std::fstream &, alert &);
	friend std::fstream & operator << (std::fstream &, alert &);
	void findPriority(std::string EnumName);
	void findSeverity(std::string EnumName);
	std::string getOrigin();
	std::string getStringSubkey();
	std::string getType();
	int getSubkey();	
	std::string getSystime();
	int operator==(alert &);
	long getDuration();
	
	
	// added "get/set" for email
	std::string getMessage();
	std::string getPriority();
	std::string getSeverity();
};
