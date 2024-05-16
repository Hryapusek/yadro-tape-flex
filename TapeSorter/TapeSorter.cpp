#include "TapeSorter.hpp"
#include <cassert>
#include <vector>
#include <fmt/format.h>
#include <filesystem>
#include <iostream>
#include <algorithm>

#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include "Utils.hpp"
#include "Strategies/ISortStrategy.hpp"
#include "Strategies/SortEnoughMemoryStrategy.hpp"
#include "Strategies/SortNotEnoughMemoryStrategy.hpp"
#include "Strategies/SortWithoutMemoryStrategy.hpp"

namespace
{
    std::shared_ptr<ISortStrategy> chooseStrategy(ITapeDevice *inputTape, const InputCharacteristics &characts)
    {
        const unsigned long maxArrayLength = TapeSorterUtils::calculateMaxArrayLength(characts);
        if (maxArrayLength >= inputTape->size())
            return std::static_pointer_cast<ISortStrategy>(std::make_shared<SortEnoughMemoryStrategy>());
        else if (std::ceil(inputTape->size() / static_cast<double>(maxArrayLength)) <= characts.reasonable_number_of_temp_tapes)
            return std::static_pointer_cast<ISortStrategy>(std::make_shared<SortNotEnoughMemoryStrategy>());
        else
            return std::static_pointer_cast<ISortStrategy>(std::make_shared<SortWithoutMemoryStrategy>());
    }
}

void TapeSorter::sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts)
{
	assert(inputTape); assert(outputTape);
    auto sortStrategy = chooseStrategy(inputTape, characts);
    sortStrategy->sort(inputTape, outputTape, characts);
}
