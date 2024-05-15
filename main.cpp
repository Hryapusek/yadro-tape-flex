#include "ConfigReader/ConfigReader.hpp"
#include "TapeSorter/TapeSorter.hpp"
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include <filesystem>
#include <iostream>
#include <memory>


int main(int argc, char const *argv[])
{
    std::filesystem::path configFile("config.txt");
    auto characteristics = ConfigReader::readFromFile(configFile);
    std::cout << "Read from config:\n" << characteristics;

    std::filesystem::path inputTapeFile("input_tape.txt");
    std::filesystem::path outputTapeFile("output_tape.txt");
    
    TapeDeviceCharacteristics inputTapeCharacteristics{
        .tape_file = inputTapeFile,
        .read_delay = characteristics.read_delay,
        .write_delay = characteristics.write_delay,
        .move_delay = characteristics.move_delay,
        .move_one_pos_delay = characteristics.move_one_pos_delay,
    };

    TapeDeviceCharacteristics outputTapeCharacteristics{
        .tape_file = outputTapeFile,
        .read_delay = characteristics.read_delay,
        .write_delay = characteristics.write_delay,
        .move_delay = characteristics.move_delay,
        .move_one_pos_delay = characteristics.move_one_pos_delay,
    };

    auto inputTape = std::make_unique<FileTapeDevice>(inputTapeCharacteristics, true);
    auto outputTape = std::make_unique<FileTapeDevice>(outputTapeCharacteristics);

    TapeSorter::sort(inputTape.get(), outputTape.get(), characteristics);
    return 0;
}

