#ifndef TAPE_SORTER_UTILS_HPP
#define TAPE_SORTER_UTILS_HPP

#include "ConfigReader/ConfigReader.hpp"
#include "TapeDevice/Interface/ITapeDevice.hpp"


class TapeSorterUtils
{
public:
    static unsigned long calculateMaxArrayLength(const InputCharacteristics &characts);
    static bool isTapeEmpty(ITapeDevice *inputTape);
    static void sortTape(ITapeDevice *tape);
};

#endif
