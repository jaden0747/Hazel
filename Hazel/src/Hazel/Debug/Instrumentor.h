#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace hazel
{

struct ProfileResult
{
  std::string name;
  long long start, end;
  uint32_t threadID;
};

struct InstrumentationSession
{
  std::string name;
};

class Instrumentor
{
private:
  InstrumentationSession *m_currentSession;
  std::ofstream m_outputStream;
  int m_profileCount;

  Instrumentor()
      : m_currentSession(nullptr), m_profileCount(0)
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
    m_outputStream << "{\"otherData\": {}, \"traceEvents\":[";
    m_outputStream.flush();
  }

  void writeFooter()
  {
    m_outputStream << "]}";
    m_outputStream.flush();
  }

  void beginSession(const std::string& name, const std::string& filepath = "results.json")
  {
    m_outputStream.open(filepath);
    writeHeader();
    m_currentSession = new InstrumentationSession({name});
  }

  void endSession()
  {
    writeFooter();
    m_outputStream.close();
    delete m_currentSession;
    m_currentSession = nullptr;
    m_profileCount = 0;
  }

  void writeProfile(const ProfileResult& result)
  {
    if (m_profileCount++ > 0)
      m_outputStream << ",";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_outputStream << "{";
    m_outputStream << "\"cat\":\"function\",";
    m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
    m_outputStream << "\"name\":\"" << name << "\",";
    m_outputStream << "\"ph\":\"X\",";
    m_outputStream << "\"pid\":0,";
    m_outputStream << "\"tid\":" << result.threadID << ",";
    m_outputStream << "\"ts\":" << result.start;
    m_outputStream << "}";

    m_outputStream.flush();
  }
};


class InstrumentationTimer
{
public:
  InstrumentationTimer(const char* name)
    : m_name(name)
    , m_stopped(false)
  {
    m_startTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~InstrumentationTimer()
  {
    if (!m_stopped)
      stop();
  }

  void stop()
  {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
    Instrumentor::get().writeProfile({m_name, start, end, threadID});

    m_stopped = true;
  }

private:
  const char* m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
  bool m_stopped;
};

#define HZ_PROFILE 1

#if HZ_PROFILE
  #define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::hazel::Instrumentor::get().beginSession(name, filepath)
  #define HZ_PROFILE_END_SESSION() ::hazel::Instrumentor::get().endSession()
  #define HZ_PROFILE_SCOPE(name) ::hazel::InstrumentationTimer timer##__LINE__(name);
  #define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
  #define HZ_PROFILE_BEGIN_SESSION(name, filepath)
  #define HZ_PROFILE_END_SESSION()
  #define HZ_PROFILE_SCOPE(name)
  #define HZ_PROFILE_FUNCTION()
#endif

}
