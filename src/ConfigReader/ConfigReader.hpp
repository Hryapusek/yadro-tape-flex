#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <string>
#include <optional>
#include <filesystem>
#include <iosfwd>

struct InputCharacteristics
{
	int read_delay_milliseconds;
	int write_delay_milliseconds;
	int move_delay_milliseconds;
	int move_one_pos_delay_milliseconds;
	unsigned long memory_bytes;
	unsigned long reasonable_number_of_temp_tapes;
	std::string input_file;
	std::string output_file;
};

std::ostream &operator<<(std::ostream &out, const InputCharacteristics characts);


class ConfigReader
{
public:
    /**
     * @throw - FieldNotFoundException if any necessary field was not found
     * @throw - FileNotFoundException
     * @throw - BadFieldException
     * @throw - std::exception if conversion to number fail
     */
	static InputCharacteristics readFromFile(std::filesystem::path file);

private:
	static void checkParameters(std::optional<int> &read_delay_milliseconds, std::optional<int> &write_delay_milliseconds,
									std::optional<int> &move_delay_milliseconds, std::optional<int> &move_one_pos_delay_milliseconds,
									std::optional<unsigned long> &memory_bytes, std::optional<std::string> &input_file,
									std::optional<std::string> &output_file, std::optional<unsigned long> &reasonable_number_of_temp_tapes);
};

#endif
