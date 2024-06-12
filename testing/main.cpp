#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	auto chn = std::make_unique<log::MockChannel_>();
	chn->RegisterDrivers({std::make_shared<log::StdConsoleDebugDriver>(std::make_unique<log::FormatterText>())});
	
	LOG.Info("test info").Channel(std::move(chn));
}