#include "include/bd.h"
#include <fstream>
#include <cstdio>
#include <ctime>
#include <exception>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstdlib>

bd::bd() {
	this->fileName = "AlertRequestHandler.txt";
}
bd::~bd() {}

std::vector<alert> bd::ReadDataBase() {
	std::vector<alert> vec;
	alert buf;
	std::fstream file(this->fileName.c_str(), std::ios::in);
	if (!file.is_open()) {
		file.close();
		return vec;
	}
	while (1)
	{
		file >> buf;                 //����������� fstream >> �� ���� ������
		if (file.eof())
			break;
		vec.push_back(buf);
	}
	return vec;
}

int bd::WriteDataBase(std::vector<alert> &vec) {
	std::vector<alert>::iterator beg = vec.begin();
	std::vector<alert>::iterator end = vec.end();
	std::fstream file(this->fileName.c_str(), std::ios::out);
	if (!file.is_open()) {
		file.close();
		return 0;
	}
	while (beg != end) {
		file << *beg;
		beg++;
	}
	return 1;
}

int bd::put(alert newAlert) {
	std::vector<alert> info = this->ReadDataBase();
	std::vector<alert>::iterator cur = info.begin(), end = info.end();		
	int fl = 0;
	if (info.empty()) {
		return -1;
	}
	while(1) {
		if (cur == end) {
    		break;
   		}
		if (compareAlert(newAlert, *cur)) {
			if (checkTime(newAlert, *cur)) {
				info.erase(cur);
				info.push_back(newAlert);
				return 1;
			}
			else {
				return 0;
			}
		}
		if (cur != end) {
			cur++;
		}
	}
	this->WriteSingleObject(newAlert);
	return 1;
}

std::vector<alert> bd::get(alert newAlert) {
	std::vector<alert> vec;
	std::vector<alert> info = this->ReadDataBase();
	std::vector<alert>::iterator cur = info.begin(), end = info.end();
	if (info.empty()) {
		return vec;
	}
	while(cur!=end) {
		if (compareAlert(newAlert, *cur)) {
			vec.push_back(*cur);
		}			
		cur++;
	}
	return vec;
}

int bd::remove(alert newAlert) {		
	std::vector<alert> info = this->ReadDataBase();
	std::vector<alert>::iterator cur = info.begin(), end = info.end();
	std::vector<alert> restore;
	int remove = 0;
	if (info.empty()) {
		return -1;
	}		
	while (cur!=end) {
		if (compareAlert(newAlert, *cur)) {				
			remove = 1;
		}
		else {
			restore.push_back(*cur);
		}
		cur++;
	}
	this->WriteDataBase(restore);
	return remove;
}

void bd::WriteSingleObject(alert newAlert) {
	std::string info;
	std::fstream file(this->fileName.c_str(), std::ios::app);
	if (!file.is_open()) {
		file.close();
		return;
	}		
	file << newAlert;
	file.close();
}	

int bd::compareAlert(alert &master, alert &slave) {
	if (master == slave) {
		return 1;
	}
	return 0;
}

int bd::checkTime(alert master, alert slave) {
	std::time_t curTime, noteTime;
	double elapsedTime;
	std::time(&curTime);
	noteTime = std::atol(slave.getSystime().c_str());
	noteTime += master.getDuration();
	elapsedTime = difftime(curTime, noteTime);		
	if (elapsedTime > 0) return 1;
	return 0;
}
