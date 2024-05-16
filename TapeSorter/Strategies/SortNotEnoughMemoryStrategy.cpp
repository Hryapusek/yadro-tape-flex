#include "SortNotEnoughMemoryStrategy.hpp"
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"
#include "TapeSorter/Utils.hpp"
#include <fmt/format.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "TapeSorter/Utils.hpp"

namespace
{
std::shared_ptr<ITapeDevice> createTempTape(unsigned long tapeSize, const InputCharacteristics &characts)
{
	static long long counter = 0;
	static constexpr auto tempFileTemplate = "tape_{}.txt";
	namespace fs = std::filesystem;
	fs::path tempDirectory = fs::current_path() / "tmp";
	if (not fs::is_directory(tempDirectory))
		fs::create_directory(tempDirectory);
	auto tempFilePath = tempDirectory / fmt::format(tempFileTemplate, counter++);
	while (fs::is_regular_file(tempFilePath))
		tempFilePath = tempDirectory / fmt::format(tempFileTemplate, counter++);

	TapeDeviceCharacteristics tempTapeCharacteristics{
		.tape_file = tempFilePath,
		.read_delay = characts.read_delay,
		.write_delay = characts.write_delay,
		.move_delay = characts.move_delay,
		.move_one_pos_delay = characts.move_one_pos_delay,
	};

	return std::static_pointer_cast<ITapeDevice>(std::make_shared<FileTapeDevice>(std::move(tempTapeCharacteristics)));
}

void saveNumbersInTempTapes(ITapeDevice *inputTape, std::vector<std::shared_ptr<ITapeDevice> > &tempTapes,
                            const InputCharacteristics &characts)
{
	try
	{
		while (true)
		{
			auto number = inputTape->read();
			tempTapes.back()->write(number);
			try
			{
				tempTapes.back()->moveForward();
			}
			catch(const EndReachedException& e)
			{
				tempTapes.push_back(createTempTape(TapeSorterUtils::calculateMaxArrayLength(characts), characts));
			}
		}
	}
	catch(const EndReachedException& e)
	{
		if (TapeSorterUtils::isTapeEmpty(tempTapes.back().get()))
		{
			tempTapes.pop_back();
		}
		return;
	}
}

void sortTempTapesAndWriteToOutput(std::vector<std::shared_ptr<ITapeDevice> > &tempTapes, ITapeDevice *outputTape)
{
	for (auto &tapeDevice : tempTapes)
	{
		try
		{
			auto numbers = tapeDevice->toVector();
			std::sort(numbers.begin(), numbers.end());
			outputTape->write(numbers);
		}
		catch(const ReadFailedException& e)
		{
			std::cerr << e.what() << '\n';
			throw;
		}
		catch(const WriteFailedException& e)
		{
			std::cerr << e.what() << '\n';
			throw;
		}
	}
}
}

void SortNotEnoughMemoryStrategy::sort(ITapeDevice * inputTape, ITapeDevice * outputTape, const InputCharacteristics & characts)
{
	std::vector<std::shared_ptr<ITapeDevice> > tempTapes;
	tempTapes.push_back(createTempTape(TapeSorterUtils::calculateMaxArrayLength(characts), characts));
	saveNumbersInTempTapes(inputTape, tempTapes, characts);
	if (tempTapes.size() == 1 and TapeSorterUtils::isTapeEmpty(tempTapes[0].get()))
	{
		std::cout << "Input tape is empty. Finishing...\n";
		return;
	}
	sortTempTapesAndWriteToOutput(tempTapes, outputTape);
	outputTape->fixSize();
	TapeSorterUtils::sortTape(outputTape);
	std::cout << "Sorting finished, sorted tape will be written to the output file soon...";
}
