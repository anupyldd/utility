/*
 * Anything with a name starting with an underscore 
 * is not supposed to be used directly.
 *
 * For logging use the EntryBuilder class 
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
		LEVEL m_level;
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
	
	class EntryBuilder : private _Entry
	{
	public:
		EntryBuilder(const std::source_location loc
					= std::source_location::current());
		EntryBuilder& Text(const std::string& txt);
		EntryBuilder& Level(LEVEL lvl);
		EntryBuilder& Channel(std::unique_ptr<_Channel> chn);
		~EntryBuilder();
	private:
		std::unique_ptr<_Channel> m_dest;
	}
	
/*****************************************************/
//				EntryBuilder Implementation
/*****************************************************/
	
	EntryBuilder::EntryBuilder(const std::source_location loc)
		: _Entry 
		{
			.m_level = LEVEL::INFO,
			.m_fileName = loc.file_name();
			.m_line = loc.line();
			.m_column = loc.column();
			.m_functionName = loc.function_name();
		}
	{
		
	}
	EntryBuilder& EntryBuilder::Text(const std::string& txt)
	{
		m_text = std::move(txt);
		return *this;
	}
	EntryBuilder& EntryBuilder::Level(LEVEL lvl)
	{
		m_level = std::move(lvl);
		return *this;
	}
	EntryBuilder& EntryBuilder::Channel(std::unique_ptr<_Channel> chn)
	{
		m_dest = std::move(chn);
		return *this;
	}
	EntryBuilder::~EntryBuilder()
	{
		if(m_dest) m_dest->Submit();
	}
}

#define LOG log::EntryBuilder{std::source_location::current()}
}