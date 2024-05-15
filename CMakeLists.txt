cmake_minimum_required(VERSION 3.11)
project(TAPE_FLEX LANGUAGE CXX)

set(option_test "Enable testing?" ON)

set(target tape_sorter)
set(test_target tape_sorter_test)
add_executable(${target})
set(sources
    ConfigReader/ConfigReader.hpp
    ConfigReader/ConfigReader.cpp
    ConfigReader/Exceptions.hpp
)
target_sources(${target} PUBLIC ${sources} main.cpp)

if(option_test)
    include(GoogleTest)
    add_executable(${test_target})
    target_compile_definitions(${test_target} PUBLIC google_test)
    target_sources(${test_target} PUBLIC ${sources} main_test.cpp)
    target_link_libraries(${test_target} PUBLIC gtest gtest_main)
endif()
