/*
 * Templated timer, requires specification of time format (TF) on creation
 * (like std::chrono::seconds or any other from chrono)
 * 
 * You can have several separate timers.
 * Give the timers different names to differentiate between them.
 *
 * Use Step() function to create intermediate steps,
 * which can then be accessed individually by name.
 *
 * Output functions convert time to string for logging.
 * Get functions return raw values.
 */
 
#include <chrono>
#include <map>
#include <string>
#include <format>
#include <sstream>
#include <list>
#include <algorithm>
#include <iomanip> 
#include <cstdlib>
#include <iostream>

namespace util
{
namespace time
{
	using ut_TimePoint = std::chrono::time_point<std::chrono::system_clock>;

/*****************************************************/
//						Timer
/*****************************************************/

	template<typename TF>
	class Timer
	{
	public:
		struct Entry
		{
			Entry(const std::string name, const ut_TimePoint& time)
				: m_name(name), m_time(time) { }
			std::string m_name;
			ut_TimePoint m_time;
		};
		using ut_StepsCont = std::list<Entry>;
		using ut_Duration = std::chrono::duration<double, std::nano>;

	public:
		inline Timer() = default;
		inline Timer(const std::string& name);
		inline ~Timer();

		inline Timer<TF>& Start();
		inline Timer<TF>& Step(const std::string& name);
		inline Timer<TF>& Finish();
		
		inline const ut_StepsCont& GetSteps() const;
		inline ut_TimePoint GetStep(const std::string& name) const;
		inline ut_TimePoint GetLast() const;
		inline auto GetDuration() const;	// from start to finish
		inline auto GetDiff(const std::string& first, const std::string& second) const; // between two time points
		
		inline std::string OutputStep(const std::string& name) const;
		inline std::string OutputLast() const;
		inline std::string OutputDuration() const;	// from start to finish
		inline std::string OutputDiff(const std::string& first, const std::string& second) const; // between two time points
		inline std::string OutputOverview() const; // formats whole list to string for logging
		
	private:
		ut_StepsCont m_steps;
		const std::string m_name = "Utility Timer";
		
	private:

		bool Contains(const std::string& name) const;
		const Entry& Find(const std::string& name) const;
	};
	
/*****************************************************/

	
	template<typename TF>
	inline Timer<TF>::Timer(const std::string& name)
		: m_name(name)
	{
	}

	template<typename TF>
	inline Timer<TF>::~Timer()
	{
		if (!Contains("TimerFinish"))
		{
			m_steps.emplace_back("TimerDestroyedUntimely", std::chrono::system_clock::now());
		}
	}

	template<typename TF>
	inline Timer<TF>& Timer<TF>::Start()
	{
		m_steps.emplace_back("TimerStart", std::chrono::system_clock::now());
		return *this;
	}

	template<typename TF>
	inline Timer<TF>& Timer<TF>::Step(const std::string& name)
	{
		m_steps.emplace_back(name, std::chrono::system_clock::now());
		return *this;
	}

	template<typename TF>
	inline Timer<TF>& Timer<TF>::Finish()
	{
		m_steps.emplace_back("TimerFinish", std::chrono::system_clock::now());
		return *this;
	}

	template<typename TF>
	inline const Timer<TF>::ut_StepsCont& Timer<TF>::GetSteps() const
	{
		return m_steps;
	}

	template<typename TF>
	inline ut_TimePoint Timer<TF>::GetStep(const std::string & name) const
	{
		return Find(name).m_time;
	}

	template<typename TF>
	inline ut_TimePoint Timer<TF>::GetLast() const
	{
		return m_steps.end()->m_time;
	}

	template<typename TF>
	inline auto Timer<TF>::GetDuration() const
	{
		return std::chrono::duration_cast<TF>(m_steps.begin()->m_time - (--m_steps.end())->m_time);
	}

	template<typename TF>
	inline auto Timer<TF>::GetDiff(const std::string& first, const std::string& second) const
	{
		return std::chrono::duration_cast<TF>(Find(first).m_time - Find(second).m_time);
	}

	template<typename TF>
	inline std::string Timer<TF>::OutputStep(const std::string& name) const
	{
		return std::format("({}) {}: {}", m_name, name, Find(name));
	}

	template<typename TF>
	inline std::string Timer<TF>::OutputLast() const
	{
		return std::format("({}) {}: {}", m_name, (--m_steps.end())->m_name, (--m_steps.end())->m_time);
	}

	template<typename TF>
	inline std::string Timer<TF>::OutputDuration() const
	{
		return std::format("({}) Timer Duration: {}", m_name, GetDuration());
	}

	template<typename TF>
	inline std::string Timer<TF>::OutputDiff(const std::string& first, const std::string& second) const
	{
		return std::format("({}) From [{}] to [{}]: {}", m_name, first, second, GetDiff(first, second));
	}

	template<typename TF>
	inline std::string Timer<TF>::OutputOverview() const
	{
		std::ostringstream oss;
		oss << std::setw(15);
		oss << "[" << m_name << "]" << '\n';

		std::string prev;
		for (const auto& elem : m_steps)
		{
			oss << std::setw(15) << elem.m_name << ": " << elem.m_time;
			if (!prev.empty()) oss << " | " << std::chrono::duration_cast<TF>(GetDiff(elem.m_name, prev)) << " from previous step";
			oss << '\n';
			prev = elem.m_name;
		}
		return oss.str();
	}

	template<typename TF>
	inline bool Timer<TF>::Contains(const std::string& name) const
	{
		return std::find_if(m_steps.begin(), m_steps.end(),
			[name](const Timer<TF>::Entry& entry) { return entry.m_name == name; }) != m_steps.end();
	}

	template<typename TF>
	inline const Timer<TF>::Entry& Timer<TF>::Find(const std::string& name) const
	{
		return *std::find_if(m_steps.begin(), m_steps.end(),
			[name](const Timer<TF>::Entry& entry) { return entry.m_name == name; });
	}



}
}