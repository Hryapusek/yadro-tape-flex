#ifndef SORT_WITHOUT_MEMORY_STRATEGY_HPP
#define SORT_WITHOUT_MEMORY_STRATEGY_HPP

#include "ISortStrategy.hpp"
#include "ConfigReader/ConfigReader.hpp"

class SortWithoutMemoryStrategy: public ISortStrategy
{
public:
    void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts) final override;
};

#endif
