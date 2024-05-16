#ifndef SORT_NOT_ENOUGH_MEMORY_STRATEGY_HPP
#define SORT_NOT_ENOUGH_MEMORY_STRATEGY_HPP

#include "ISortStrategy.hpp"
#include "ConfigReader/ConfigReader.hpp"

class SortNotEnoughMemoryStrategy: public ISortStrategy
{
public:
    void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts) final override;
};

#endif
