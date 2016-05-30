#include <iostream>
#include <string>
#include "Config.hpp"


int main() {
	Config buf,buf1;
	buf.ReadConfig();
	std::string line;
	buf.MakeNote();	
	//buf.MakeTable();
	/*
	buf.EnterInf();
	buf.EnterInf("/a.out");
	buf.EnterInf("/a.out", "math");
	*/
	//buf.EnterInf("/b.out", "math", 14);
	//buf.WriteConfig();		
	//line = buf.GetMessage("/c.out");
	//std::cout << line;
	
	buf.SetGlobal();
	std::cout << buf.GetMessage() << std::endl;
	std::cout << buf.GetMessage("/a.out") << std::endl;
	std::cout << buf.GetMessage("/a.out","math") << std::endl;
	std::cout << buf.GetMessage("/a.out", "math", 1) << std::endl;
	std::cout << "**************" << std::endl;
	std::cout << buf.GetPeriodicity() << std::endl;
	std::cout << buf.GetPeriodicity("/c.out") << std::endl;
	std::cout << buf.GetPeriodicity("/a.out", "fork") << std::endl;
	std::cout << buf.GetPeriodicity("/a.out", "math", 1) << std::endl;
	std::cout << "**************" << std::endl;
	std::cout << buf.GetPriority() << std::endl;
	std::cout << buf.GetPriority("/x.out") << std::endl;
	std::cout << buf.GetPriority("/a.out","er") << std::endl;
	std::cout << buf.GetPriority("/a.out","mer",1) << std::endl;
	std::cout << "**************" << std::endl;
	std::cout << buf.GetSeverity() << std::endl;
	std::cout << buf.GetSeverity("/c.out") << std::endl;
	std::cout << buf.GetSeverity("/a.out", "zzzzz") << std::endl;
	std::cout << buf.GetSeverity("/a.out", "math", 125) << std::endl;
	
	return 0;
}