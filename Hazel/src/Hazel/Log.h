#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace hazel 
{

class HAZEL_API Log {
public:
    Log();
   ~Log();

    static void init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_core_logger_; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_client_logger_; }
    
private:
    static std::shared_ptr<spdlog::logger> s_core_logger_;
    static std::shared_ptr<spdlog::logger> s_client_logger_;
};

}

//! CORE LOG MACRO

#define HZ_CORE_CRITICAL(...) ::hazel::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define HZ_CORE_ERROR(...)    ::hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_WARN(...)     ::hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_INFO(...)     ::hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_DEBUG(...)    ::hazel::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define HZ_CORE_TRACE(...)    ::hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)

//! CLIENT LOG MACRO

#define HZ_CRITICAL(...) ::hazel::Log::GetClientLogger()->critical(__VA_ARGS__)
#define HZ_ERROR(...)    ::hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_WARN(...)     ::hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_INFO(...)     ::hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_DEBUG(...)    ::hazel::Log::GetClientLogger()->debug(__VA_ARGS__)
#define HZ_TRACE(...)    ::hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
