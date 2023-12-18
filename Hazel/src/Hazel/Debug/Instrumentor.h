#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <fstream>
#include <thread>

namespace hazel
{

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult
{
  std::string name;
  FloatingPointMicroseconds start;
  std::chrono::microseconds elapsedTime;
  std::thread::id threadID;
};

struct InstrumentationSession
{
  std::string name;
};

class Instrumentor
{
private:
  std::mutex m_mutex;
  InstrumentationSession *m_currentSession;
  std::ofstream m_outputStream;

  Instrumentor()
      : m_currentSession(nullptr)
  {
  }

public:
  static Instrumentor& get()
  {
    static Instrumentor instance;
    return instance;
  }

  void writeHeader()
  {
    m_outputStream << "{\"otherData\": {}, \"traceEvents\":[{}";
    m_outputStream.flush();
  }

  void writeFooter()
  {
    m_outputStream << "]}";
    m_outputStream.flush();
  }

  void internalEndSession()
  {
    if (m_currentSession)
    {
      writeFooter();
      m_outputStream.close();
      delete m_currentSession;
      m_currentSession = nullptr;
    }
  }

  void beginSession(const std::string& name, const std::string& filepath = "results.json")
  {
    std::lock_guard lock(m_mutex);
    if (m_currentSession)
    {
      if (Log::getCoreLogger())
      {
        HZ_CORE_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open.", name, m_currentSession->name);
      }
      internalEndSession();
    }
    m_outputStream.open(filepath);
    if (m_outputStream.is_open())
    {
      m_currentSession = new InstrumentationSession({name});
      writeHeader();
    }
    else
    {
      if (Log::getCoreLogger())
      {
        HZ_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
      }
    }
    m_currentSession = new InstrumentationSession({name});
  }

  void endSession()
  {
    std::lock_guard lock(m_mutex);
    internalEndSession();
  }

  void writeProfile(const ProfileResult& result)
  {
    std::stringstream json;

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    json << std::setprecision(3) << std::fixed;
    json << ",{";
    json << "\"cat\":\"function\",";
    json << "\"dur\":" << (result.elapsedTime.count()) << ',';
    json << "\"name\":\"" << name << "\",";
    json << "\"ph\":\"X\",";
    json << "\"pid\":0,";
    json << "\"tid\":" << result.threadID << ",";
    json << "\"ts\":" << result.start.count();
    json << "}";

    std::lock_guard lock(m_mutex);
    if (m_currentSession)
    {
      m_outputStream << json.str();
      m_outputStream.flush();
    }
  }
};


class InstrumentationTimer
{
public:
  InstrumentationTimer(const char* name)
    : m_name(name)
    , m_stopped(false)
  {
    m_startTimepoint = std::chrono::steady_clock::now();
  }

  ~InstrumentationTimer()
  {
    if (!m_stopped)
      stop();
  }

  void stop()
  {
    auto endTimepoint = std::chrono::steady_clock::now();
    auto highResStart = FloatingPointMicroseconds{m_startTimepoint.time_since_epoch()};
    auto elapsedTime =
      std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
      std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch();

    Instrumentor::get().writeProfile({m_name, highResStart, elapsedTime, std::this_thread::get_id()});

    m_stopped = true;
  }

private:
  const char* m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
  bool m_stopped;
};

	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define HZ_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define HZ_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
		#define HZ_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define HZ_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define HZ_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define HZ_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define HZ_FUNC_SIG __func__
	#else
		#define HZ_FUNC_SIG "HZ_FUNC_SIG unknown!"
	#endif

#define HZ_PROFILE 0

#if HZ_PROFILE
  #define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::hazel::Instrumentor::get().beginSession(name, filepath)
  #define HZ_PROFILE_END_SESSION() ::hazel::Instrumentor::get().endSession()
  #define HZ_PROFILE_SCOPE(name) ::hazel::InstrumentationTimer timer##__LINE__(name);
  #define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(HZ_FUNC_SIG)
#else
  #define HZ_PROFILE_BEGIN_SESSION(name, filepath)
  #define HZ_PROFILE_END_SESSION()
  #define HZ_PROFILE_SCOPE(name)
  #define HZ_PROFILE_FUNCTION()
#endif

}
