#include "ConfigReader.hpp"
#include <gtest/gtest.h>

TEST(config, default_config_read)
{
	std::filesystem::path path_to_config("test/config.txt");
	auto inputCharacteristics = ConfigReader::readFromFile(path_to_config.string());
	EXPECT_EQ(inputCharacteristics.read_delay, 1.25);
	EXPECT_EQ(inputCharacteristics.write_delay, 0.0005);
	EXPECT_EQ(inputCharacteristics.move_delay, 0.4);
	EXPECT_EQ(inputCharacteristics.move_one_pos_delay, 0.001);
	EXPECT_EQ(inputCharacteristics.memory_bytes, 10ul);
	EXPECT_EQ(inputCharacteristics.reasonable_number_of_temp_tapes, 100000ul);
	EXPECT_EQ(inputCharacteristics.input_file, "input_tape.txt");
	EXPECT_EQ(inputCharacteristics.output_file, "output_tape.txt");
	
}
