#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "cpp/cad/core/ports/LoggerPort.hpp"

namespace cad::adapters::spdlog {

class SpdlogAdapter : public cad::ports::LoggerPort {
public:
  explicit SpdlogAdapter(std::shared_ptr<::spdlog::logger> logger = nullptr);
  ~SpdlogAdapter() override = default;

  void log(cad::ports::LogLevel level, const std::string &message) override;

private:
  std::shared_ptr<::spdlog::logger> logger_;
  
  ::spdlog::level::level_enum mapLogLevel(cad::ports::LogLevel level) const;
};

} // namespace cad::adapters::spdlog
