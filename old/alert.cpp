#include "include/alert.h"
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cstdio>

alert::alert() {	
	this->prior.push_back("HIGH");
	this->prior.push_back("MEDIUM");
	this->prior.push_back("LOW");
	this->sever.push_back("CRITICAL");
	this->sever.push_back("ERROR");
	this->sever.push_back("WARNING");
	this->sever.push_back("DEBUG");

	this->k.origin = "./c.out";
	this->k.type = "fork()";
	this->k.subkey = 123;
	this->node.pr = HIGH;
	this->node.sev = DEBUG;
	this->node.state = "OLD";
	this->node.limTime.lday    = 0;
	this->node.limTime.lhour   = 0;
	this->node.limTime.lminute = 0;
	this->node.limTime.lsecond = 0;
	this->node.message = "mes1";	
}

std::string alert::GetLocalTime() {
	std::string Time;
	char str[6];
	time_t t;
	struct tm *t_m;
	t = time(NULL);
	t_m = localtime(&t);

	sprintf(str, "%02d%c", t_m->tm_mday, ';');
	Time += str;
	sprintf(str, "%02d%c", t_m->tm_mon, ';');
	Time += str;
	sprintf(str, "%04d%c", t_m->tm_year, ';');
	Time += str;
	sprintf(str, "%02d%c", t_m->tm_hour, ';');
	Time += str;
	sprintf(str, "%02d%c", t_m->tm_min, ';');
	Time += str;
	sprintf(str, "%02d", t_m->tm_sec);
	Time += str;
	return Time;
}

std::string alert::getStringSubkey() {
	std::string info;
	info = this->IntToString(this->k.subkey);
	return info;
}

std::string alert::getTime() {
 std::string info;
 info = this->IntToString(this->node.curTime.day) + '.' + this->IntToString(this->node.curTime.month) + '.' +
     this->IntToString(this->node.curTime.year) + ' ' + this->IntToString(this->node.curTime.hour) + ':' +
     this->IntToString(this->node.curTime.minute) + ':' + this->IntToString(this->node.curTime.second);
 return info;
}

std::string alert::IntToString(int value) {
	std::string result;
	std::ostringstream buf;
	buf << value;
	result = buf.str();
	return result;
}

std::string alert::AllTime() {
	std::string Time;	
	char str[25];
	time_t szClock;
	time(&szClock);
	sprintf(str, "%013ld", szClock);
	Time += str;
	return Time;
}


std::string alert::serialize(char n) {
	std::string buf;
	buf += n;
	buf += ';';
	buf += this->k.origin;
	buf += ';';
	buf += this->k.type;
	buf += ';';
	buf += this->IntToString(this->k.subkey);
	buf += ';';
	buf += this->prior[this->node.pr];
	buf += ';';
	buf += this->sever[this->node.sev];
	buf += ';';
	buf += this->node.state;
	buf += ';';
	buf += this->IntToString(this->node.limTime.lday);
	buf += ';';
	buf += this->IntToString(this->node.limTime.lhour);
	buf += ';';
	buf += this->IntToString(this->node.limTime.lminute);
	buf += ';';
	buf += this->IntToString(this->node.limTime.lsecond);
	buf += ';';
	buf += this->node.message;
	buf += ';';
	buf += this->GetLocalTime();
	buf += ';';
	buf += this->AllTime();
	buf += '*';
	return buf;
}

int alert::initFromString(std::string info) { 
	if (info.empty()) 
		return -1; 
	std::string buf; 
	std::stringstream ff; 
	char sym; 
	int mode; 
	ff << info; 
	getline(ff, buf, ';'); 
	sym = buf[0]; 
	mode = (int)sym; 
	getline(ff, buf, ';'); 
	this->k.origin = buf.c_str(); 
	getline(ff, buf, ';'); 
	this->k.type = buf.c_str(); 
	getline(ff, buf, ';'); 
	this->k.subkey = atoi(buf.c_str()); //можно неявно 
	getline(ff, buf, ';'); 
	this->findPriority(buf); 
	getline(ff, buf, ';'); 
	this->findSeverity(buf); 
	getline(ff, buf, ';'); 
	this->node.state = buf.c_str(); 
	getline(ff, buf, ';'); 
	this->node.limTime.lday = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.limTime.lhour = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.limTime.lminute = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.limTime.lsecond = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.message = buf.c_str(); 
	getline(ff, buf, ';'); 
	this->node.curTime.day = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.curTime.month = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.curTime.year = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.curTime.hour = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.curTime.minute = atoi(buf.c_str()); 
	getline(ff, buf, ';'); 
	this->node.curTime.second = atoi(buf.c_str()); 
	getline(ff, buf, '*'); 
	this->node.sysTime = buf.c_str(); 
	return mode; 
}


std::fstream & operator << (std::fstream &file, alert & newAlert) {
	file << newAlert.k.origin << ';';
	file << newAlert.k.type << ';';
	file << newAlert.k.subkey << ';';
	file << newAlert.prior[newAlert.node.pr] << ';';
	file << newAlert.sever[newAlert.node.sev] << ';';
	file << newAlert.node.state << ';';
	file << newAlert.node.limTime.lday << ';';
	file << newAlert.node.limTime.lhour << ';';
	file << newAlert.node.limTime.lminute << ';';
	file << newAlert.node.limTime.lsecond << ';';
	file << newAlert.node.message << ';';
	file << newAlert.GetLocalTime() << ';';
	file << newAlert.AllTime() << '*';
	return file;
}

//OUT
std::fstream & operator >>(std::fstream &file, alert &newAlert) {
	if (file.peek() == EOF)
		return file;	
	std::string  ss, buf;
	getline(file, ss, '*');	
	std::stringstream ff;
	ff << ss;
	getline(ff, buf, ';');
	newAlert.k.origin = buf.c_str();	
	getline(ff, buf, ';');
	newAlert.k.type = buf.c_str();
	getline(ff, buf, ';');
	newAlert.k.subkey = atoi(buf.c_str());  
	getline(ff, buf, ';');
	newAlert.findPriority(buf);
	getline(ff, buf, ';');
	newAlert.findSeverity(buf);
	getline(ff, buf, ';');
	newAlert.node.state = buf.c_str();
	getline(ff, buf, ';');
	newAlert.node.limTime.lday = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.limTime.lhour = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.limTime.lminute = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.limTime.lsecond = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.message = buf.c_str();
	getline(ff, buf, ';');
	newAlert.node.curTime.day = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.curTime.month = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.curTime.year = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.curTime.hour = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.curTime.minute = atoi(buf.c_str());
	getline(ff, buf, ';');
	newAlert.node.curTime.second = atoi(buf.c_str());
	getline(ff, buf, '*');
	newAlert.node.sysTime = buf.c_str();
	return file;
}


void alert::findPriority(std::string EnumName) {
	if (EnumName == "HIGH") {
		this->node.pr = HIGH;
	}
	if (EnumName == "MEDIUM") {
		this->node.pr = MEDIUM;
	}
	if (EnumName == "LOW") {
		this->node.pr = LOW;
	}
}

void alert::findSeverity(std::string EnumName) {
	if (EnumName == "CRITICAL") {
		this->node.sev = CRITICAL;
	}
	if (EnumName == "ERROR") {
		this->node.sev = ERROR;
	}
	if (EnumName == "WARNING") {
		this->node.sev = WARNING;
	}
	if (EnumName == "DEBUG") {
		this->node.sev = DEBUG;
	}
}

std::string alert::getOrigin() {
	return this->k.origin;
}

std::string alert::getType() {
	return this->k.type;
}

int alert::getSubkey() {
	return this->k.subkey;
}

std::string alert::getSystime() {
	return this->node.sysTime;
}

int alert::operator==(alert &cur) {
	int fl = 0;
	if (this->k.origin.empty() || cur.k.origin.empty()) {
		fl = 1;
	}
	else if (this->k.origin != cur.k.origin) {
		return 0;
	}
	if (this->k.type.empty() || cur.k.type.empty()) {
		fl = 1;
	} else if (this->k.type != cur.k.type) {
		return 0;
	}
	if (this->k.subkey == -1 || cur.k.subkey == -1) {
		fl = 1;
	} else if (this->k.subkey != cur.k.subkey) {
	  return 0;
	}
	return 1;
}
	
long alert::getDuration() {
	return this->node.limTime.lday * 86400 + 
				 this->node.limTime.lhour * 3600 + 
				 this->node.limTime.lminute * 60 + 
				 this->node.limTime.lsecond;
}

std::string alert::getMessage() {
	return this->node.message;
}

std::string alert::getPriority() {	
	switch (this->node.pr) {
		case HIGH:
			return "HIGH";
		case MEDIUM:
			return "HIGH";
		case LOW:
			return "HIGH";
		default:
			return "";
	}
}

std::string alert::getSeverity() {	
	switch (this->node.pr) {
		case CRITICAL:
			return "CRITICAL";
		case ERROR:
			return "ERROR";
		case WARNING:
			return "WARNING";
		case DEBUG:
			return "DEBUG";
		default:
			return "";
	}
}