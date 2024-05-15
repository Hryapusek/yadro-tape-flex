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
}
