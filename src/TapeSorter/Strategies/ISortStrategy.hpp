#ifndef ISORT_STRATEGY_HPP
#define ISORT_STRATEGY_HPP

#include "TapeDevice/Interface/ITapeDevice.hpp"
#include "ConfigReader/ConfigReader.hpp"

class ISortStrategy
{
public:
    virtual void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts) = 0;
};

#endif
