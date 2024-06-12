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
 */

#pragma once

#include <iostream>
#include <chrono>
#include <memory>
#include <vector>
#include <format>
#include <filesystem>
#include <fstream>
#include <source_location>

namespace util
{
namespace log
{
	enum class LOG_LEVEL
	{
		NONE,
		FATAL,
		ERROR,
		WARN,
		INFO,
		DEBUG,
		TRACE
	};
	std::string GetLOG_LEVELName_(LOG_LEVEL lvl)
	{
		switch(lvl)
		{
			case LOG_LEVEL::TRACE: return "TRACE";
			case LOG_LEVEL::DEBUG: return "DEBUG";
			case LOG_LEVEL::INFO: return "INFO";
			case LOG_LEVEL::WARN: return "WARN";
			case LOG_LEVEL::ERROR: return "ERROR";
			case LOG_LEVEL::FATAL: return "FATAL";
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
		LOG_LEVEL m_level = LOG_LEVEL::INFO;
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
		SeverityPolicy(LOG_LEVEL LOG_LEVEL);
		bool TransformEntry(const Entry_& entry) override;
		LOG_LEVEL m_level;
	};
	
/*****************************************************/

	SeverityPolicy::SeverityPolicy(LOG_LEVEL LOG_LEVEL)
		: m_level(LOG_LEVEL) { }
		
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
			GetLOG_LEVELName_(entry.m_level), 
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
	
	struct FileDriver : public DriverFmt_
	{
		FileDriver(const std::filesystem::path& path);
		FileDriver(const std::filesystem::path& path, std::unique_ptr<Formatter_> fmtr);
		void SetFormatter(std::unique_ptr<Formatter_> fmtr) override;
		void Submit(const Entry_& entry) override;
		
		std::ofstream m_file;
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
	
// file output
	FileDriver::FileDriver(const std::filesystem::path& path)
		: DriverFmt_(std::make_unique<FormatterText>()) 
	{
		std::filesystem::create_directories(path.parent_path());
		m_file.open(path, m_file.out | m_file.app);
	}
		
	FileDriver::FileDriver(const std::filesystem::path& path, std::unique_ptr<Formatter_> fmtr)
		: DriverFmt_(std::move(fmtr)) 
	{
		std::filesystem::create_directories(path.parent_path());
		m_file.open(path, m_file.out | m_file.app);
	}

	void FileDriver::SetFormatter(std::unique_ptr<Formatter_> fmtr)
	{
		m_formatter = std::move(fmtr);
	}
	
	void FileDriver::Submit(const Entry_& entry)
	{
		if(m_formatter)
		{
			m_file << m_formatter->Format(entry);
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
		for(auto& plc : m_policies)
		{
			if(!plc->TransformEntry(entry)) 
			{
				return;
			}
		}
		for(const auto& drv : m_drivers)
		{
			drv->Submit(entry);
		}
	}
	void ChannelBase_::RegisterDrivers(std::initializer_list<std::shared_ptr<Driver_>> drvs)
	{
		m_drivers.insert(m_drivers.end(), drvs.begin(), drvs.end());
	}
	void ChannelBase_::RegisterPolicies(std::initializer_list<std::shared_ptr<Policy_>> plcs)
	{
		m_policies.insert(m_policies.end(), plcs.begin(), plcs.end());
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
		EntryBuilder_& Level(LOG_LEVEL lvl);
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
	EntryBuilder_& EntryBuilder_::Level(LOG_LEVEL lvl)
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
		Level(LOG_LEVEL::TRACE).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Debug(const std::string& txt)
	{
		Level(LOG_LEVEL::DEBUG).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Info(const std::string& txt)
	{
		Level(LOG_LEVEL::INFO).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Warn(const std::string& txt)
	{
		Level(LOG_LEVEL::WARN).Text(txt);
		return *this;
	}
	EntryBuilder_& EntryBuilder_::Fatal(const std::string& txt)
	{
		Level(LOG_LEVEL::FATAL).Text(txt);
		return *this;
	}
	
	EntryBuilder_::~EntryBuilder_()
	{
		if(m_dest) m_dest->Submit(*this);
	}
	
	
}	// namespace log

/*****************************************************/
//				Pre-defined functions for logging
/*****************************************************/

	const std::filesystem::path DEFAULT_FILE_LOG_PATH = "log\\log.txt";

	void ConsoleLog(const std::string& text, log::LOG_LEVEL lvl)
	{
		auto chn = std::make_unique<log::ChannelDefault>();
		chn->RegisterDrivers({std::make_shared<log::StdConsoleDebugDriver>()});
		chn->RegisterPolicies({std::make_shared<log::SeverityPolicy>(lvl)});
		auto eb = std::make_unique<log::EntryBuilder_>(std::source_location::current());
		eb->Channel(std::move(chn));
		eb->Level(lvl);
		eb->Text(text);
	}	

	void ConsoleLogTrace(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::TRACE);
	}
	void ConsoleLogDebug(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::DEBUG);
	}
	void ConsoleLogInfo(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::INFO);
	}
	void ConsoleLogWarn(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::WARN);
	}
	void ConsoleLogError(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::ERROR);
	}
	void ConsoleLogFatal(const std::string& text)
	{
		ConsoleLog(text, log::LOG_LEVEL::FATAL);
	}
	
	void FileLog(const std::string& text, const std::filesystem::path& path, log::LOG_LEVEL lvl)
	{
		auto chn = std::make_unique<log::ChannelDefault>();
		chn->RegisterDrivers({std::make_shared<log::FileDriver>(path)});
		chn->RegisterPolicies({std::make_shared<log::SeverityPolicy>(lvl)});
		auto eb = std::make_unique<log::EntryBuilder_>(std::source_location::current());
		eb->Channel(std::move(chn));
		eb->Level(lvl);
		eb->Text(text);
	}
	
	void FileLogTrace(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::TRACE);
	}
	void FileLogDebug(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::DEBUG);
	}
	void FileLogInfo(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::INFO);
	}
	void FileLogWarn(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::WARN);
	}
	void FileLogError(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::ERROR);
	}
	void FileLogFatal(const std::string& text, const std::filesystem::path& path)
	{
		FileLog(text, path, log::LOG_LEVEL::FATAL);
	}
	
	void FileLogTrace(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::TRACE);
	}
	void FileLogDebug(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::DEBUG);
	}
	void FileLogInfo(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::INFO);
	}
	void FileLogWarn(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::WARN);
	}
	void FileLogError(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::ERROR);
	}
	void FileLogFatal(const std::string& text)
	{
		FileLog(text, DEFAULT_FILE_LOG_PATH, log::LOG_LEVEL::FATAL);
	}
	
	
}	// namespace util 