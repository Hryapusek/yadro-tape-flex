#include "ConfigReader.hpp"
#include <gtest/gtest.h>

TEST(config, default_config_read)
{
	std::filesystem::path path_to_config(std::filesystem::current_path() / "test/config.txt");
	auto inputCharacteristics = ConfigReader::readFromFile(path_to_config.string());
	EXPECT_EQ(inputCharacteristics.read_delay_milliseconds, 100);
	EXPECT_EQ(inputCharacteristics.write_delay_milliseconds, 100);
	EXPECT_EQ(inputCharacteristics.move_delay_milliseconds, 100);
	EXPECT_EQ(inputCharacteristics.move_one_pos_delay_milliseconds, 100);
	EXPECT_EQ(inputCharacteristics.memory_bytes, 10ul);
	EXPECT_EQ(inputCharacteristics.reasonable_number_of_temp_tapes, 100000ul);
	EXPECT_EQ(inputCharacteristics.input_file, "input_tape.txt");
	EXPECT_EQ(inputCharacteristics.output_file, "output_tape.txt");
	EXPECT_EQ(inputCharacteristics.reasonable_number_of_temp_tapes, 100000);
}
