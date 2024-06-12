/*
 * Anything with a name that has an underscore 
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
	};
	std::string GetLevelName_(LEVEL lvl)
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
	
	struct Entry_
	{
		Entry_(const std::source_location& src)
			: m_source(src) { }
		LEVEL m_level = LEVEL::INFO;
		std::string m_text;
		std::source_location m_source;
		std::chrono::system_clock::time_point m_timestamp;
	};
	
	class Channel_
	{
	public:
		virtual void Submit(Entry_& entry);
		virtual ~Channel_() = default;
	};
	
	class MockChannel : public Channel_
	{
	public:
		Entry_ mEntry_;
		void Submit(Entry_& entry) override final
		{
			// TODO
		}
	};
	
/*****************************************************/
//				Main Logging Class
/*****************************************************/
	
	class EntryBuilder_ : private Entry_
	{
	public:
		EntryBuilder_(const std::source_location loc
					= std::source_location::current());
		
		EntryBuilder_& Text(const std::string& txt);
		EntryBuilder_& Level(LEVEL lvl);
		EntryBuilder_& Channel(std::unique_ptr<Channel_> chn);
		
		EntryBuilder_& Trace(const std::string& txt);
		EntryBuilder_& Debug(const std::string& txt);
		EntryBuilder_& Info(const std::string& txt);
		EntryBuilder_& Warn(const std::string& txt);
		EntryBuilder_& Fatal(const std::string& txt);
		
		~EntryBuilder_();
		
	private:
		std::unique_ptr<Channel_> m_dest;
	};
	
/*****************************************************/
//				EntryBuilder_ Implementation
/*****************************************************/
	
	EntryBuilder_::EntryBuilder_(const std::source_location loc)
		: Entry_(loc)
	{
		
	}
	EntryBuilder_& EntryBuilder_::Text(const std::string& txt)
	{
		m_text = std::move(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Level(LEVEL lvl)
	{
		m_level = std::move(lvl);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Channel(std::unique_ptr<Channel_> chn)
	{
		m_dest = std::move(chn);
		return *this;
	}
	// TODO
	EntryBuilder_& EntryBuilder_::Trace(const std::string& txt)
	{
		Level(LEVEL::TRACE).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Debug(const std::string& txt)
	{
		Level(LEVEL::DEBUG).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Info(const std::string& txt)
	{
		Level(LEVEL::INFO).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Warn(const std::string& txt)
	{
		Level(LEVEL::WARN).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Fatal(const std::string& txt)
	{
		Level(LEVEL::FATAL).Text(txt);
		return *this;
	}
	
	EntryBuilder_::~EntryBuilder_()
	{
		if(m_dest) m_dest->Submit(*this);
	}
}

#define LOG log::EntryBuilder_{std::source_location::current()}
}