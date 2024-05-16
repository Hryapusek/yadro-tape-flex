#include "SortWithoutMemoryStrategy.hpp"
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include "TapeSorter/Utils.hpp"
#include <fmt/format.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "TapeSorter/Utils.hpp"

namespace
{
void copy(ITapeDevice * inputTape, ITapeDevice * outputTape)
{
	if (TapeSorterUtils::isTapeEmpty(inputTape))
		return;
	inputTape->moveToBegin();
	outputTape->moveToBegin();
	try
	{
		while(true)
		{
			auto number = inputTape->read();
			outputTape->write(number);
			inputTape->moveForward();
			outputTape->moveForward();
		}
	}
	catch(const EndReachedException& e)
	{
		return;
	}
}
}

void SortWithoutMemoryStrategy::sort(ITapeDevice * inputTape, ITapeDevice * outputTape, const InputCharacteristics & characts)
{
	if (TapeSorterUtils::isTapeEmpty(inputTape))
	{
		std::cout << "Input tape is empty. Finishing...\n";
		return;
	}
	copy(inputTape, outputTape);
	TapeSorterUtils::sortTape(outputTape);
	std::cout << "Sorting finished, sorted tape will be written to the output file soon...";
}
