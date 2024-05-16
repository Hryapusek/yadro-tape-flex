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
		.read_delay_milliseconds = characts.read_delay_milliseconds,
		.write_delay_milliseconds = characts.write_delay_milliseconds,
		.move_delay_milliseconds = characts.move_delay_milliseconds,
		.move_one_pos_delay_milliseconds = characts.move_one_pos_delay_milliseconds,
	};

	return std::static_pointer_cast<ITapeDevice>(std::make_shared<FileTapeDevice>(std::move(tempTapeCharacteristics), tapeSize));
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
			inputTape->moveForward();
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
			std::ranges::sort(numbers);
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
	TapeSorterUtils::sortTape(outputTape);
	std::cout << "Sorting finished, sorted tape will be written to the output file soon...";
}
