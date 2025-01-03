#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Hazel/Core/Base.h"

namespace hazel
{

class Log
{
public:
    static void init();

    static Ref<spdlog::logger>& getCoreLogger()
    {
        return s_core_logger_;
    }
    static Ref<spdlog::logger>& getClientLogger()
    {
        return s_client_logger_;
    }

private:
    static Ref<spdlog::logger> s_core_logger_;
    static Ref<spdlog::logger> s_client_logger_;
};

} // namespace hazel

//! CORE LOG MACRO

#define HZ_CORE_CRITICAL(...) ::hazel::Log::getCoreLogger()->critical(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::hazel::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_WARN(...) ::hazel::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_INFO(...) ::hazel::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_DEBUG(...) ::hazel::Log::getCoreLogger()->debug(__VA_ARGS__)
#define HZ_CORE_TRACE(...) ::hazel::Log::getCoreLogger()->trace(__VA_ARGS__)

//! CLIENT LOG MACRO

#define HZ_CRITICAL(...) ::hazel::Log::getClientLogger()->critical(__VA_ARGS__)
#define HZ_ERROR(...) ::hazel::Log::getClientLogger()->error(__VA_ARGS__)
#define HZ_WARN(...) ::hazel::Log::getClientLogger()->warn(__VA_ARGS__)
#define HZ_INFO(...) ::hazel::Log::getClientLogger()->info(__VA_ARGS__)
#define HZ_DEBUG(...) ::hazel::Log::getClientLogger()->debug(__VA_ARGS__)
#define HZ_TRACE(...) ::hazel::Log::getClientLogger()->trace(__VA_ARGS__)
