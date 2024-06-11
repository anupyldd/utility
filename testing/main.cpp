#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	
	log::MockChannel mc;
	
	LOG.Level(log::LEVEL::FATAL).Text("testest").Channel(&mc);
}