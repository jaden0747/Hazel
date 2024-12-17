#include "Hazel/Core/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "hzpch.h"

namespace hazel
{
Ref<spdlog::logger> Log::s_core_logger_;
Ref<spdlog::logger> Log::s_client_logger_;

void Log::init()
{
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Hazel.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_core_logger_ = Ref<spdlog::logger>(new spdlog::logger("HAZEL", begin(logSinks), end(logSinks)));
    spdlog::register_logger(s_core_logger_);
    s_core_logger_->set_level(spdlog::level::trace);
    s_core_logger_->flush_on(spdlog::level::trace);

    s_client_logger_ = Ref<spdlog::logger>(new spdlog::logger("APP", begin(logSinks), end(logSinks)));
    spdlog::register_logger(s_client_logger_);
    s_client_logger_->set_level(spdlog::level::trace);
    s_client_logger_->flush_on(spdlog::level::trace);
}
}  // namespace hazel
