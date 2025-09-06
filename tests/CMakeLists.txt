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
target_include_directories(test_usecase PRIVATE ${CMAKE_SOURCE_DIR}/cpp/cad)

include(Catch)
catch_discover_tests(test_usecase)

add_test(NAME cli_mem_demo COMMAND $<TARGET_FILE:cad_cli> list mem:demo)
