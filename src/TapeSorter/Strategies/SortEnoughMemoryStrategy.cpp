#include "SortEnoughMemoryStrategy.hpp"
#include <algorithm>

void SortEnoughMemoryStrategy::sort(ITapeDevice * inputTape, ITapeDevice * outputTape, const InputCharacteristics & characts)
{
    auto numbers = inputTape->toVector();
    std::ranges::sort(numbers);
    outputTape->write(numbers);
}
