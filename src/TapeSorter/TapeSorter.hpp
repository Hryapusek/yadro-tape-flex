#ifndef TAPE_SORTER_HPP
#define TAPE_SORTER_HPP

#include "ConfigReader/ConfigReader.hpp"
#include "TapeDevice/Interface/ITapeDevice.hpp"
#include <filesystem>

class TapeSorter
{
public:
    static void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts);
};

#endif
