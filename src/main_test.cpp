#include <gtest/gtest.h>
#include "ConfigReader/ConfigReader.hpp"
#include "TapeSorter/TapeSorter.hpp"
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include <filesystem>
#include <iostream>
#include <memory>


TEST(default_case, lot_memory)
{
    namespace fs = std::filesystem;
    fs::path configFile("test_cases/lot_memory_case/config.txt");
    auto characteristics = ConfigReader::readFromFile(configFile);
    std::cout << "Read from config:\n" << characteristics;

    fs::path inputTapeFile(fs::current_path() / characteristics.input_file);
    fs::path outputTapeFile(fs::current_path() / characteristics.output_file);
    
    TapeDeviceCharacteristics inputTapeCharacteristics{
        .tape_file = inputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    TapeDeviceCharacteristics outputTapeCharacteristics{
        .tape_file = outputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    {
        auto inputTape = std::make_unique<FileTapeDevice>(inputTapeCharacteristics);
        auto outputTape = std::make_unique<FileTapeDevice>(outputTapeCharacteristics, inputTape->size());

        TapeSorter::sort(inputTape.get(), outputTape.get(), characteristics);
    }
    fs::path outputTapeFilePath(fs::current_path() / "test_cases/lot_memory_case/output_tape.txt");
    FileTapeDevice outputTapeDevice({.tape_file = outputTapeFilePath});

    std::vector<unsigned> outputTapeContents = outputTapeDevice.toVector();
    bool isSorted = std::is_sorted(outputTapeContents.begin(), outputTapeContents.end());
    EXPECT_TRUE(isSorted);
}

TEST(default_case, little_memory)
{
    namespace fs = std::filesystem;
    fs::path configFile("test_cases/little_memory_case/config.txt");
    auto characteristics = ConfigReader::readFromFile(configFile);
    std::cout << "Read from config:\n" << characteristics;

    fs::path inputTapeFile(fs::current_path() / characteristics.input_file);
    fs::path outputTapeFile(fs::current_path() / characteristics.output_file);
    
    TapeDeviceCharacteristics inputTapeCharacteristics{
        .tape_file = inputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    TapeDeviceCharacteristics outputTapeCharacteristics{
        .tape_file = outputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    {
        auto inputTape = std::make_unique<FileTapeDevice>(inputTapeCharacteristics);
        auto outputTape = std::make_unique<FileTapeDevice>(outputTapeCharacteristics, inputTape->size());

        TapeSorter::sort(inputTape.get(), outputTape.get(), characteristics);
    }
    fs::path outputTapeFilePath(fs::current_path() / "test_cases/little_memory_case/output_tape.txt");
    FileTapeDevice outputTapeDevice({.tape_file = outputTapeFilePath});

    std::vector<unsigned> outputTapeContents = outputTapeDevice.toVector();
    bool isSorted = std::is_sorted(outputTapeContents.begin(), outputTapeContents.end());
    EXPECT_TRUE(isSorted);
}

TEST(default_case, no_memory)
{
    namespace fs = std::filesystem;
    fs::path configFile("test_cases/no_memory_case/config.txt");
    auto characteristics = ConfigReader::readFromFile(configFile);
    std::cout << "Read from config:\n" << characteristics;

    fs::path inputTapeFile(fs::current_path() / characteristics.input_file);
    fs::path outputTapeFile(fs::current_path() / characteristics.output_file);
    
    TapeDeviceCharacteristics inputTapeCharacteristics{
        .tape_file = inputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    TapeDeviceCharacteristics outputTapeCharacteristics{
        .tape_file = outputTapeFile,
        .read_delay_milliseconds = characteristics.read_delay_milliseconds,
        .write_delay_milliseconds = characteristics.write_delay_milliseconds,
        .move_delay_milliseconds = characteristics.move_delay_milliseconds,
        .move_one_pos_delay_milliseconds = characteristics.move_one_pos_delay_milliseconds,
    };

    {
        auto inputTape = std::make_unique<FileTapeDevice>(inputTapeCharacteristics);
        auto outputTape = std::make_unique<FileTapeDevice>(outputTapeCharacteristics, inputTape->size());

        TapeSorter::sort(inputTape.get(), outputTape.get(), characteristics);
    }
    fs::path outputTapeFilePath(fs::current_path() / "test_cases/no_memory_case/output_tape.txt");
    FileTapeDevice outputTapeDevice({.tape_file = outputTapeFilePath});

    std::vector<unsigned> outputTapeContents = outputTapeDevice.toVector();
    bool isSorted = std::is_sorted(outputTapeContents.begin(), outputTapeContents.end());
    EXPECT_TRUE(isSorted);
}

int main(int argc, char const **argv)
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

