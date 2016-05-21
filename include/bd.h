#include "alert.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <exception>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class bd {
private:
private:
	std::string fileName;
public:
	bd();
	~bd() {}
	std::vector<alert> bd::ReadDataBase();
	int WriteDataBase(std::vector<alert>);
	int put(alert);
	int get(alert);
	int remove(alert);	
};