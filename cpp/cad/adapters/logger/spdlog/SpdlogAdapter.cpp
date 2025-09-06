#include "cpp/cad/adapters/logger/spdlog/SpdlogAdapter.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/common.h>

namespace cad::adapters::spdlog {

SpdlogAdapter::SpdlogAdapter(std::shared_ptr<::spdlog::logger> logger)
    : logger_(logger) {
  if (!logger_) {
    // Create a default console logger if none provided
    logger_ = ::spdlog::stdout_color_mt("cad_logger");
    logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    // Colors are automatically enabled with stdout_color_mt
  }
}

void SpdlogAdapter::log(cad::ports::LogLevel level, const std::string &message) {
  logger_->log(mapLogLevel(level), message);
}

::spdlog::level::level_enum SpdlogAdapter::mapLogLevel(cad::ports::LogLevel level) const {
  switch (level) {
    case cad::ports::LogLevel::Trace:
      return ::spdlog::level::trace;
    case cad::ports::LogLevel::Debug:
      return ::spdlog::level::debug;
    case cad::ports::LogLevel::Info:
      return ::spdlog::level::info;
    case cad::ports::LogLevel::Warn:
      return ::spdlog::level::warn;
    case cad::ports::LogLevel::Error:
      return ::spdlog::level::err;
    default:
      return ::spdlog::level::info;
  }
}

} // namespace cad::adapters::spdlog
