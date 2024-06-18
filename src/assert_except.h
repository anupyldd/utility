/*
 * Custom assertions and exceptions.
 
 * This is a modified version of an assertion and exception system created 
 * by ChiliTomatoNoodle in his Game Engine Infrastructure series
 * (https://www.youtube.com/playlist?list=PLqCJpWy5FoheHDzaP3R1eDFDOOff5TtBA).
*/

#include <string>
#include <source_location>
#include <iostream>
#include <sstream>
#include <exception>
#include <format>

// concatination, conversion to string or wstring,
// the ones with underscore are helpers
#define UT_CAT_(a, b) a##b
#define UT_CAT(a, b) UT_CAT_(a, b)
#define UT_TOSTR_(a) #a
#define UT_TOSTR(a) UT_TOSTR_(a)
#define UT_TOWSTR(a) UT_CAT(L, UT_TOSTR(a))

// assert output, uses util::ConsoleLog if present,
// other defaults to std::cout
/*#ifdef UT_LOGGER_PRESENT
#define UT_ASRT_LOG(str) util::ConsoleLog(str, util::log::LOG_LEVEL::FATAL); std::terminate()
#else
#define UT_ASRT_LOG(str) std::cout << str << '\n'; std::terminate()
#endif*/

/*****************************************************/

namespace util
{	
namespace ae
{
/*****************************************************/
//				Assertion
/*****************************************************/
	
	class Assertion
	{
	public:
		Assertion(const std::string& expr, const char* file, const char* func, int line);
		~Assertion();
		Assertion& Msg(const std::string& msg);
		template<typename T>
		Assertion& Watch(T&& val, const std::string& name);
	private:
		const char* m_file = nullptr;
		const char* m_func = nullptr;
		int m_line = -1;
		std::ostringstream m_stream;
	};
	
/*****************************************************/

	Assertion::Assertion(const std::string& expr, const char* file, const char* func, int line)
		: m_file(file), m_func(func), m_line(line)
	{
		m_stream << "Assertion failed: " << expr << '\n';
	}
	
	Assertion::~Assertion()
	{
		std::cout << std::format("{}    File: {}({}): {}\n",
								m_stream.str(),
								m_file, m_line, m_func);
		exit(1);
	}
	
	Assertion& Assertion::Msg(const std::string& msg)
	{
		m_stream << "    Message: " << msg << '\n';
		return *this;
	}
	
	template<typename T>
	Assertion& Assertion::Watch(T&& val, const std::string& name)
	{
		m_stream << "    Name: " << std::forward<T>(val) << '\n';
		return *this;
	}
}
}

// for controlling what to include in release or debug
#ifndef UT_ACTIVE
#ifdef NOT_DEBUG
#define UT_ACTIVE false
#else
#define UT_ACTIVE true
#endif
#endif

#define UT_ASSERT(expr) (!UT_ACTIVE || bool(expr)) ? void(0) : (void)util::ae::Assertion{UT_TOSTR(expr), __FILE__, __FUNCTION__, __LINE__}
