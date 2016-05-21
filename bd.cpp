#include "bd.h"

class bd {
private:
private:
	std::string fileName;
public:

	bd::bd() {
		this->fileName = "AlertRequestHandler.txt";
	}
	bd::~bd() {}

	std::vector<alert> bd::ReadDataBase() {
		std::vector<alert> vec;
		alert buf;
		std::fstream file(this->fileName, std::ios::in);
		if (!file.is_open()) {
			file.close();
			return vec;
		}
		while (1)
		{
			file >> buf;                 //перегрузить fstream >> во всех гаджет
			if (file.eof())
				break;
			vec.push_back(buf);
		}
		return vec;
	}

	int bd::WriteDataBase(std::vector<alert> &vec) {
		std::vector<alert>::iterator beg = vec.begin();
		std::vector<alert>::iterator end = vec.end();
		std::fstream file(this->fileName, std::ios::out);
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
		if (info.empty()) {
			return -1;
		}

		return 0;
	}

	int bd::get(alert newAlert) {
		return 0;
	}

	int bd::remove(alert newAlert) {
		return 0;
	}

	void bd::WriteSingleObject(alert newAlert) {
		std::string info;
		std::fstream file(this->fileName, std::ios::app);
		if (!file.is_open()) {
			file.close();
			return;
		}		
		file << newAlert;
		file.close();
	}	

	int bd::compareAlert(alert master, alert slave) {
		if (master == slave) {
			return 1;
		}
		return 0;
	}
};