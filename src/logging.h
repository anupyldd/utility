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
#include <vector>
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
		
/*****************************************************/
//				Entry
/*****************************************************/
	
	struct Entry_
	{
		Entry_() = default;
		Entry_(const std::source_location& src)
			: m_source(src) { }
		LEVEL m_level = LEVEL::INFO;
		std::string m_text;
		std::source_location m_source;
		std::chrono::system_clock::time_point m_timestamp;
	};
	
/*****************************************************/
//				Driver
/*****************************************************/

	class Driver_
	{
	public:
		virtual void Submit(const Entry_& entry) = 0;
		virtual ~Driver_() = default;
	};

/*****************************************************/
//				Channel
/*****************************************************/
	
	class Channel_
	{
	public:
		virtual void Submit(Entry_& entry) = 0;
		virtual void RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs) = 0;
		virtual ~Channel_() = default;
	};
	
	class ChannelBase_ : public Channel_
	{
	public:
		ChannelBase_() = default;
		ChannelBase_(std::vector<std::shared_ptr<Driver_>> drvs);
		virtual void Submit(Entry_& entry) override;
		virtual void RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs) override;
	private:
		std::vector<std::shared_ptr<Driver_>> m_drivers;
	};
	
/*****************************************************/
	
	ChannelBase_::ChannelBase_(std::vector<std::shared_ptr<Driver_>> drvs)
		: m_drivers(std::move(drvs)) { }
		
	void ChannelBase_::Submit(Entry_& entry)
	{
		for(const auto& drv : m_drivers)
		{
			drv->Submit(entry);
			std::cout << "Submit to driver\n";
		}
	}
	void ChannelBase_::RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs)
	{
		m_drivers.insert(m_drivers.end(), drvs.begin(), drvs.end());
		std::cout << "Registered " << drvs.size() << " drivers\n";
	}
	
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
	
	
/*****************************************************/
// 			Temp
/*****************************************************/
	
	
	class MockChannel_ : public ChannelBase_
	{
	public:
		/*void RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs)
		{
			
		}
		void Submit(Entry_& entry) override
		{
			std::cout << "Test from MockChannel\n";
			//std::cout << "Number of drivers: " << m_drivers.size() << '\n';
		}*/
	};
	
	class MockDriver_ : public Driver_
	{
		virtual void Submit(const Entry_& entry) override
		{
			std::cout << "Test from MockDriver\n";
		}
	};
}

#define LOG log::EntryBuilder_{std::source_location::current()}
}