cmake_minimum_required(VERSION 3.16)

add_library(cad_core INTERFACE)
target_include_directories(cad_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

add_library(cad_usecases)
target_sources(cad_usecases PRIVATE core/usecase/ListModelPartsUseCase.cpp)
target_include_directories(cad_usecases PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(cad_usecases PUBLIC cad_core)

add_library(adapter_fake)
target_sources(
  adapter_fake
  PRIVATE adapters/cad-model-reader/fake/FakeCadModelReaderAdapter.cpp
          adapters/model-data-source/fake/FakeModelDataSourceAdapter.cpp
          adapters/logger/fake/FakeLoggerAdapter.cpp)
target_include_directories(adapter_fake PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(adapter_fake PUBLIC cad_core)

# Find packages
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(adapter_spdlog)
target_sources(
  adapter_spdlog
  PRIVATE adapters/logger/spdlog/SpdlogAdapter.cpp)
target_include_directories(adapter_spdlog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(adapter_spdlog PUBLIC cad_core spdlog::spdlog)

add_library(adapter_json)
target_sources(
  adapter_json
  PRIVATE adapters/model-data-source/json/JsonModelDataSourceAdapter.cpp
          adapters/cad-model-reader/json/JsonCadModelReaderAdapter.cpp)
target_include_directories(adapter_json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(adapter_json PUBLIC cad_core nlohmann_json::nlohmann_json)

add_executable(cad_cli app/cli/main.cpp app/cli/Formatter.cpp)
target_include_directories(cad_cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})
target_link_libraries(cad_cli PRIVATE cad_usecases adapter_fake adapter_spdlog adapter_json)
