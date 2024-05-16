#ifndef SORT_ENOUGH_MEMORY_STRATEGY_HPP
#define SORT_ENOUGH_MEMORY_STRATEGY_HPP

#include "ISortStrategy.hpp"

class SortEnoughMemoryStrategy: public ISortStrategy
{
public:
    void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts) final override;
};

#endif
