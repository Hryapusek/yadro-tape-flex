cmake_minimum_required(VERSION 3.11)
project(TAPE_FLEX)

option(option_test "Enable testing?" ON)

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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(target tape_sorter)
set(test_target tape_sorter_test)
add_executable(${target})
set(sources
    src/ConfigReader/ConfigReader.hpp
    src/ConfigReader/ConfigReader.cpp
    src/ConfigReader/Exceptions.hpp
    src/TapeDevice/FileTapeDevice/FileTapeDevice.hpp
    src/TapeDevice/Interface/ITapeDevice.hpp
    src/TapeSorter/TapeSorter.cpp
    src/TapeSorter/TapeSorter.hpp
    src/TapeSorter/Strategies/ISortStrategy.hpp
    src/TapeSorter/Strategies/SortEnoughMemoryStrategy.cpp
    src/TapeSorter/Strategies/SortEnoughMemoryStrategy.hpp
    src/TapeSorter/Strategies/SortNotEnoughMemoryStrategy.cpp
    src/TapeSorter/Strategies/SortNotEnoughMemoryStrategy.hpp
    src/TapeSorter/Strategies/SortWithoutMemoryStrategy.cpp
    src/TapeSorter/Strategies/SortWithoutMemoryStrategy.hpp
    src/TapeSorter/Utils.cpp
    src/TapeSorter/Utils.hpp
    src/TapeDevice/FileTapeDevice/FileTapeDevice.cpp
    src/TapeDevice/Interface/Exceptions.hpp
)
set(libs fmt)
if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX)
else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
endif()

target_sources(${target} PUBLIC ${sources} src/main.cpp)
target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${target} PUBLIC ${libs})

if(option_test)
    include(GoogleTest)
    add_executable(${test_target})
    set(test_sources
    src/ConfigReader/TestConfigReader.cpp
    )
    target_sources(${test_target} PUBLIC ${sources} ${test_sources} src/main_test.cpp)
    target_include_directories(${test_target} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${test_target} PUBLIC ${libs} gtest gtest_main)
    file(COPY ${CMAKE_SOURCE_DIR}/src/ConfigReader/test DESTINATION ${CMAKE_BINARY_DIR}/)
    file(COPY ${CMAKE_SOURCE_DIR}/test_cases DESTINATION ${CMAKE_BINARY_DIR}/)
endif()
