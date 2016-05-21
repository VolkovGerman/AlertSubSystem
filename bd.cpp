#include "bd.h"

class bd {
private:
private:
	std::string fileName;
public:

	bd() {
	}
	~bd() {}

	std::vector<alert> ReadDataBase();
	int WriteDataBase(std::vector<alert>);
	int put(alert);
	int get(alert);
	int remove(alert);
	

};