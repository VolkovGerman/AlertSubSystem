#include <iostream>
#include <stdio.h>
#include "alertSubSystem.h"
#include "alert.h"

int main(){
	int a, b;
	double div;
	char symb;
	
	do {
		std::cout << "Input a:" << std::endl;
		std::cin >> a;
		
		std::cout << "Input b:" << std::endl;
		std::cin >> b;
		
		if (b == 0) {
			// That's bad, send an email
			AlertSubSystem::put_alert();
		} else {
			div = a / b;
			std::cout << "The result of the division is " << div << std::endl; 
		}
		
		std::cout << "Would you like to calculate again? (y/n)";
		std::cin >> symb;
	} while (symb == 'y');
	
	return 0;
}
