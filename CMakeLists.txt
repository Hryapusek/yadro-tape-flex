cmake_minimum_required(VERSION 3.11)
project(TAPE_FLEX)

set(option_test "Enable testing?" ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG a4715c4
)
FetchContent_MakeAvailable(fmt)

set(target tape_sorter)
set(test_target tape_sorter_test)
add_executable(${target})
set(sources
    ConfigReader/ConfigReader.hpp
    ConfigReader/ConfigReader.cpp
    ConfigReader/Exceptions.hpp
    TapeDevice/FileTapeDevice/FileTapeDevice.hpp
    TapeDevice/Interface/ITapeDevice.hpp
    TapeSorter/TapeSorter.cpp
    TapeSorter/TapeSorter.hpp
    TapeDevice/FileTapeDevice/FileTapeDevice.cpp
)
set(libs fmt)
target_sources(${target} PUBLIC ${sources} main.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(${target} PUBLIC ${libs})

if(option_test)
    include(GoogleTest)
    add_executable(${test_target})
    set(test_sources
        ConfigReader/TestConfigReader.cpp
    )
    target_compile_definitions(${test_target} PUBLIC google_test)
    target_sources(${test_target} PUBLIC ${sources} ${test_sources} main_test.cpp)
    target_include_directories(${test_target} PUBLIC ${CMAKE_SOURCE_DIR})
    target_link_libraries(${test_target} PUBLIC ${libs} gtest gtest_main)
    file(COPY ${CMAKE_SOURCE_DIR}/ConfigReader/test DESTINATION ${CMAKE_BINARY_DIR}/)
endif()
