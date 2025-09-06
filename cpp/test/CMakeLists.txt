cmake_minimum_required(VERSION 3.16)

find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
  find_package(Catch2 2 REQUIRED)
  add_definitions(-DCATCH_CONFIG_ENABLE_BENCHMARKING=0)
endif()

add_executable(test_usecase usecase/ListModelPartsUseCase.test.cpp)
if(Catch2_VERSION VERSION_LESS 3)
  target_link_libraries(test_usecase PRIVATE cad_usecases adapter_fake
                                             Catch2::Catch2)
else()
  target_link_libraries(test_usecase PRIVATE cad_usecases adapter_fake
                                             Catch2::Catch2WithMain)
endif()
target_include_directories(test_usecase PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(test_spdlog_adapter logger/SpdlogAdapter.test.cpp)
if(Catch2_VERSION VERSION_LESS 3)
  target_link_libraries(test_spdlog_adapter PRIVATE adapter_spdlog
                                                    Catch2::Catch2)
else()
  target_link_libraries(test_spdlog_adapter PRIVATE adapter_spdlog
                                                    Catch2::Catch2WithMain)
endif()
target_include_directories(test_spdlog_adapter PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(test_json_model_data_source model-data-source/JsonModelDataSourceAdapter.test.cpp)
if(Catch2_VERSION VERSION_LESS 3)
  target_link_libraries(test_json_model_data_source PRIVATE adapter_json
                                                            Catch2::Catch2)
else()
  target_link_libraries(test_json_model_data_source PRIVATE adapter_json
                                                            Catch2::Catch2WithMain)
endif()
target_include_directories(test_json_model_data_source PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(test_json_cad_model_reader cad-model-reader/JsonCadModelReaderAdapter.test.cpp)
if(Catch2_VERSION VERSION_LESS 3)
  target_link_libraries(test_json_cad_model_reader PRIVATE adapter_json
                                                           Catch2::Catch2)
else()
  target_link_libraries(test_json_cad_model_reader PRIVATE adapter_json
                                                           Catch2::Catch2WithMain)
endif()
target_include_directories(test_json_cad_model_reader PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(test_opencascade_cad_model_reader cad-model-reader/OpenCascadeCadModelReaderAdapter.test.cpp)
if(Catch2_VERSION VERSION_LESS 3)
  target_link_libraries(test_opencascade_cad_model_reader PRIVATE adapter_opencascade
                                                                  Catch2::Catch2)
else()
  target_link_libraries(test_opencascade_cad_model_reader PRIVATE adapter_opencascade
                                                                  Catch2::Catch2WithMain)
endif()
target_include_directories(test_opencascade_cad_model_reader PRIVATE ${CMAKE_SOURCE_DIR})

include(Catch)
catch_discover_tests(test_usecase)
catch_discover_tests(test_spdlog_adapter)
catch_discover_tests(test_json_model_data_source)
catch_discover_tests(test_json_cad_model_reader)
catch_discover_tests(test_opencascade_cad_model_reader)
