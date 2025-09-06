#include <catch2/catch_all.hpp>
#include <spdlog/sinks/ostream_sink.h>
#include <sstream>

#include "cpp/cad/adapters/logger/spdlog/SpdlogAdapter.hpp"

TEST_CASE("SpdlogAdapter logs messages with correct levels") {
  // Create an ostringstream sink to capture log output
  auto oss = std::make_shared<std::ostringstream>();
  auto ostream_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(*oss);
  
  // Create a logger with the ostream sink
  auto logger = std::make_shared<spdlog::logger>("test_logger", ostream_sink);
  logger->set_pattern("%l: %v"); // Simple pattern: level: message
  logger->set_level(spdlog::level::trace); // Ensure all levels are logged
  
  cad::adapters::spdlog::SpdlogAdapter adapter(logger);

  SECTION("Logs trace level messages") {
    adapter.log(cad::ports::LogLevel::Trace, "trace message");
    REQUIRE(oss->str().find("trace: trace message") != std::string::npos);
  }

  SECTION("Logs debug level messages") {
    adapter.log(cad::ports::LogLevel::Debug, "debug message");
    REQUIRE(oss->str().find("debug: debug message") != std::string::npos);
  }

  SECTION("Logs info level messages") {
    adapter.log(cad::ports::LogLevel::Info, "info message");
    REQUIRE(oss->str().find("info: info message") != std::string::npos);
  }

  SECTION("Logs warn level messages") {
    adapter.log(cad::ports::LogLevel::Warn, "warn message");
    REQUIRE(oss->str().find("warning: warn message") != std::string::npos);
  }

  SECTION("Logs error level messages") {
    adapter.log(cad::ports::LogLevel::Error, "error message");
    REQUIRE(oss->str().find("error: error message") != std::string::npos);
  }
}

TEST_CASE("SpdlogAdapter creates default logger when none provided") {
  // This test verifies that the adapter can work without a provided logger
  cad::adapters::spdlog::SpdlogAdapter adapter;
  
  // Should not throw an exception
  REQUIRE_NOTHROW(adapter.log(cad::ports::LogLevel::Info, "test message"));
}
