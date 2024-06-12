#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	LOG.Info("test info").Channel(std::make_unique<log::MockChannel>());
}