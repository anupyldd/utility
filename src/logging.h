/*
 * Anything with a name starting with an underscore 
 * is not supposed to be used directly.
 *
 * For logging use the defined LOG macros class 
 */

#pragma once

#include <iostream>
#include <chrono>
#include <memory>
#include <source_location>

namespace util
{
namespace log
{

	enum class LEVEL
	{
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL
	}
	std::string _GetLevelName(LEVEL lvl)
	{
		switch(lvl)
		{
			case LEVEL::TRACE: return "TRACE";
			case LEVEL::DEBUG: return "DEBUG";
			case LEVEL::INFO: return "INFO";
			case LEVEL::WARN: return "WARN";
			case LEVEL::FATAL: return "FATAL";
			default: return "UNKNOWN_LEVEL";
		}
	}
	
	class _Entry
	{
		LEVEL m_level = LEVEL::INFO;
		std::string m_text,
					m_fileName,
					m_line,
					m_column,
					m_functionName;
		std::chrono::system_clock::time_point m_timestamp;
	}
	
	class _Channel
	{
	public:
		virtual void Submit(_Entry& entry);
		virtual ~Submit() = default;
	}
	
	class MockChannel : public _Channel
	{
	public:
		_Entry m_entry;
		void Submit(_Entry& entry) override final
		{
			// TODO
		}
	}
	
/*****************************************************/
//				Main Logging Class
/*****************************************************/
	
	class _EntryBuilder : private _Entry
	{
	public:
		_EntryBuilder(const std::source_location loc
					= std::source_location::current());
		
		_EntryBuilder& Text(const std::string& txt);
		_EntryBuilder& Level(LEVEL lvl);
		_EntryBuilder& Channel(std::unique_ptr<_Channel> chn);
		
		_EntryBuilder& Trace(const std::string& txt);
		_EntryBuilder& Debug(const std::string& txt);
		_EntryBuilder& Info(const std::string& txt);
		_EntryBuilder& Warn(const std::string& txt);
		_EntryBuilder& Fatal(const std::string& txt);
		
		~_EntryBuilder();
		
	private:
		std::unique_ptr<_Channel> m_dest;
	}
	
/*****************************************************/
//				_EntryBuilder Implementation
/*****************************************************/
	
	_EntryBuilder::_EntryBuilder(const std::source_location loc)
		: _Entry 
		{
			.m_fileName = loc.file_name();
			.m_line = loc.line();
			.m_column = loc.column();
			.m_functionName = loc.function_name();
		}
	{
		
	}
	_EntryBuilder& _EntryBuilder::Text(const std::string& txt)
	{
		m_text = std::move(txt);
		return *this;
	}
	_EntryBuilder& _EntryBuilder::Level(LEVEL lvl)
	{
		m_level = std::move(lvl);
		return *this;
	}
	_EntryBuilder& _EntryBuilder::Channel(std::unique_ptr<_Channel> chn)
	{
		m_dest = std::move(chn);
		return *this;
	}
	// TODO
	_EntryBuilder& Trace(const std::string& txt)
	{
		Level(LEVEL::TRACE).Text(txt);
		return *this;
	}
	_EntryBuilder& Debug(const std::string& txt)
	{
		Level(LEVEL::DEBUG).Text(txt);
		return *this;
	}
	_EntryBuilder& Info(const std::string& txt)
	{
		Level(LEVEL::INFO).Text(txt);
		return *this;
	}
	_EntryBuilder& Warn(const std::string& txt)
	{
		Level(LEVEL::WARN).Text(txt);
		return *this;
	}
	_EntryBuilder& Fatal(const std::string& txt)
	{
		Level(LEVEL::FATAL).Text(txt);
		return *this;
	}
	
	_EntryBuilder::~_EntryBuilder()
	{
		if(m_dest) m_dest->Submit();
	}
}

#define LOG log::_EntryBuilder{std::source_location::current()}
}