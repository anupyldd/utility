#include <iostream>

#include "../src/logging.h"

int main()
{
	using namespace util;
	
	ConsoleLog("trace", log::LOG_LEVEL::TRACE);
	ConsoleLog("debug", log::LOG_LEVEL::DEBUG);
	ConsoleLog("infor", log::LOG_LEVEL::INFO);
	ConsoleLog("warnn", log::LOG_LEVEL::WARN);
	ConsoleLog("error", log::LOG_LEVEL::ERROR);
	ConsoleLog("fatal", log::LOG_LEVEL::FATAL);
	
	FileLog("trace in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::TRACE);
	FileLog("debug in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::DEBUG);
	FileLog("infor in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::INFO);
	FileLog("warnn in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::WARN);
	FileLog("error in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::ERROR);
	FileLog("fatal in file", DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::FATAL);
	
	std::cout << "[END OF MAIN]\n";
	std::cin.get();
	
}