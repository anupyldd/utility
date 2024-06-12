#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	
	Log("info info");
	std::cout << "[END OF MAIN]\n";
}