/*
 * Anything with a name that has an underscore 
 * is not supposed to be used directly.
 *
 * For logging use the defined logging functions:
 * TODO : list of functions 
 *
 * To create a custom channel inherit the ChannelBase_ class.
 * To create a custom driver inherit:
 * - the Driver_ class (if you don't want log entries to be formatted);
 * - the DriverFmt_ class (if you want log entries to be formatted).
 * To create a custom formatter, inherit the Formatter_ class.
 * To see how to make a custom logging function, see the implementation
 * of already defined functions in the end of the file.
 *
 * This logger is a modified version of a log system created 
 * by ChiliTomatoNoodle in his Game Engine Infrastructure series
 * (https://www.youtube.com/playlist?list=PLqCJpWy5FoheHDzaP3R1eDFDOOff5TtBA).
 * Changes include:
 * - use of more modern C++ features (like std::source_location);
 * - additional drivers;
 * - different text color for each severity level in console output;
 * - switch from macros to functions for logging.
 * - small stylistic changes
 */

#pragma once

#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <format>
#include <source_location>

namespace util
{
namespace log
{

	enum LEVEL
	{
		NONE,
		FATAL,
		ERROR,
		WARN,
		INFO,
		DEBUG,
		TRACE
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
			default: return "NONE";
		}
	}
		
/*****************************************************/
//				Entry
/*****************************************************/
	
	struct Entry_
	{
		Entry_() = default;
		Entry_(const std::source_location& src, std::chrono::system_clock::time_point ts)
			: m_source(src), m_timestamp(ts) { }
		LEVEL m_level = LEVEL::INFO;
		std::string m_text;
		std::source_location m_source;
		std::chrono::system_clock::time_point m_timestamp;
	};

/*****************************************************/
//				Policy
/*****************************************************/
	
	struct Policy_
	{
		virtual bool TransformEntry(const Entry_& entry) = 0;
		virtual ~Policy_() = default;
	};
	
	struct SeverityPolicy : public Policy_
	{
		SeverityPolicy(LEVEL level);
		bool TransformEntry(const Entry_& entry) override;
		LEVEL m_level;
	};
	
/*****************************************************/

	SeverityPolicy::SeverityPolicy(LEVEL level)
		: m_level(level) { }
		
	bool SeverityPolicy::TransformEntry(const Entry_& entry)
	{
		return entry.m_level >= m_level;
	}

/*****************************************************/
//				Formatter
/*****************************************************/

	struct Formatter_ 
	{
		virtual ~Formatter_() = default;
		virtual std::string Format(const Entry_& entry) const = 0;	
	};
	
	struct FormatterText : public Formatter_
	{
		std::string Format(const Entry_& entry) const override;
	};
	
/*****************************************************/

	std::string FormatterText::Format(const Entry_& entry) const
	{
		return std::format("[{}] ({}) : \"{}\" in function: {}\n   {}({})\n", 
			GetLevelName_(entry.m_level), 
			std::chrono::zoned_time{std::chrono::current_zone(), entry.m_timestamp},
			entry.m_text,
			entry.m_source.function_name(),
			entry.m_source.file_name(),
			entry.m_source.line()
			);
	}

/*****************************************************/
//				Drivers
/*****************************************************/

	// does not format entries
	struct Driver_
	{
		virtual void Submit(const Entry_& entry) = 0;
		virtual ~Driver_() = default;
	};
	
	// formats entries
	struct DriverFmt_ : public Driver_
	{
		DriverFmt_(std::unique_ptr<Formatter_> fmtr) 
			: m_formatter(std::move(fmtr)) { }
		virtual void Submit(const Entry_& entry) = 0;
		virtual void SetFormatter(std::unique_ptr<Formatter_> fmtr) = 0;
		std::unique_ptr<Formatter_> m_formatter;
	};
	
	struct StdConsoleDebugDriver : public DriverFmt_
	{
		StdConsoleDebugDriver();
		StdConsoleDebugDriver(std::unique_ptr<Formatter_> fmtr);
		void SetFormatter(std::unique_ptr<Formatter_> fmtr) override;
		void Submit(const Entry_& entry) override;
	};

/*****************************************************/
// standard console output
	StdConsoleDebugDriver::StdConsoleDebugDriver()
		: DriverFmt_(std::make_unique<FormatterText>()) { }

	StdConsoleDebugDriver::StdConsoleDebugDriver(std::unique_ptr<Formatter_> fmtr)
		: DriverFmt_(std::move(fmtr)) { }
		
	void StdConsoleDebugDriver::SetFormatter(std::unique_ptr<Formatter_> fmtr)
	{
		m_formatter = std::move(fmtr);
	}
	
	void StdConsoleDebugDriver::Submit(const Entry_& entry)
	{
		if(m_formatter)
		{
			std::cout << m_formatter->Format(entry);
		}
	}
/*****************************************************/
//				Channel
/*****************************************************/
	
	struct Channel_
	{
		virtual void Submit(Entry_& entry) = 0;
		virtual void RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs) = 0;
		virtual void RegisterPolicies(std::initializer_list<std::shared_ptr<Policy_>> plcs) = 0;
		virtual ~Channel_() = default;
	};
	
	class ChannelBase_ : public Channel_
	{
	public:
		ChannelBase_() = default;
		ChannelBase_(std::vector<std::shared_ptr<Driver_>> drvs);
		void Submit(Entry_& entry) override;
		void RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs) override;
		void RegisterPolicies(std::initializer_list<std::shared_ptr<Policy_>> plcs) override;
	private:
		std::vector<std::shared_ptr<Driver_>> m_drivers;
		std::vector<std::shared_ptr<Policy_>> m_policies;
	};
	
	class ChannelDefault : public ChannelBase_
	{
		
	};
	
/*****************************************************/
	
	ChannelBase_::ChannelBase_(std::vector<std::shared_ptr<Driver_>> drvs)
		: m_drivers(std::move(drvs)) { }
		
	void ChannelBase_::Submit(Entry_& entry)
	{
		std::cout << "channel::submit\n";
		std::cout << m_policies.size() << " <pl-dr> " << m_drivers.size() << '\n';
		for(auto& plc : m_policies)
		{
			std::cout << "check policy\n";
			if(!plc->TransformEntry(entry)) 
			{
				std::cout << "TransformEntry check failed\n";
				return;
			}
		}
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
	void ChannelBase_::RegisterPolicies(std::initializer_list<std::shared_ptr<Policy_>> plcs)
	{
		m_policies.insert(m_policies.end(), plcs.begin(), plcs.end());
		std::cout << "Registered " << plcs.size() << " policies\n";
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
		: Entry_(loc, std::chrono::system_clock::now())
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
	
}

	void Log(const std::string& text)
	{
		auto chn = std::make_unique<log::ChannelDefault>();
		chn->RegisterDrivers({std::make_shared<log::StdConsoleDebugDriver>()});
		chn->RegisterPolicies({std::make_shared<log::SeverityPolicy>(log::LEVEL::NONE)});
		auto eb = std::make_unique<log::EntryBuilder_>(std::source_location::current());
		std::cout << "created eb\n";
		eb->Channel(std::move(chn));
		std::cout << "added chan\n";
		eb->Info(text);
		std::cout << "logged\n";
	}	
//#define LOG log::EntryBuilder_{std::source_location::current()}.Channel(std::make_unique<log::ChannelDefault>())
//#define LOGTO log::EntryBuilder_{std::source_location::current()}
}