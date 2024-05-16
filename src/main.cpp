#include "ConfigReader/ConfigReader.hpp"
#include "TapeSorter/TapeSorter.hpp"
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include <filesystem>
#include <iostream>
#include <memory>


int main()
{
    namespace fs = std::filesystem;
    fs::path configFile("config.txt");
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

    auto inputTape = std::make_unique<FileTapeDevice>(inputTapeCharacteristics);
    auto outputTape = std::make_unique<FileTapeDevice>(outputTapeCharacteristics, inputTape->size());

    TapeSorter::sort(inputTape.get(), outputTape.get(), characteristics);
    return 0;
}

