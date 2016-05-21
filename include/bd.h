#include "alert.h"

class bd {
private:
	std::string fileName;
public:
	bd();
	~bd();
	std::vector<alert> ReadDataBase();
	int WriteDataBase(std::vector<alert> &);
	int put(alert);
	std::vector<alert> get(alert);
	int remove(alert, alert &);
	void WriteSingleObject(alert);
	int compareAlert(alert, alert);
        int checkTime(alert, alert);
};
