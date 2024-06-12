#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	auto chn = std::make_unique<log::MockChannel_>();
	auto drv1 = std::make_shared<log::MockDriver_>();
	auto drv2 = std::make_shared<log::MockDriver_>();
	chn->RegisterDrivers({drv1, drv2});
	
	LOG.Info("test info").Channel(std::move(chn));
}