#include "TapeSorter.hpp"
#include <cassert>
#include <vector>
#include <fmt/format.h>
#include <filesystem>
#include "TapeDevice/FileTapeDevice/FileTapeDevice.hpp"


namespace
{
    std::shared_ptr<ITapeDevice> createTempTape(unsigned long tapeSize, const InputCharacteristics &characts)
    {
        static long long counter = 0;
        static const std::string tempFileTemplate = "tape_{}.txt";
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
}

void TapeSorter::sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts)
{
    assert(inputTape); assert(outputTape);
    std::vector<std::shared_ptr<ITapeDevice>> tempTapes;
    const unsigned long maxArrayLength = characts.memory_bytes / 4;
    tempTapes.push_back(createTempTape(maxArrayLength, characts));
}
