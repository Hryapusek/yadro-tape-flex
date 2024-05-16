#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <string>
#include <optional>
#include <filesystem>
#include <iosfwd>

struct InputCharacteristics
{
	double read_delay;
	double write_delay;
	double move_delay;
	double move_one_pos_delay;
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
	static void checkParameters(std::optional<double> &read_delay, std::optional<double> &write_delay,
								std::optional<double> &move_delay, std::optional<double> &move_one_pos_delay,
								std::optional<unsigned long> &memory_bytes, std::optional<std::string> &input_file,
								std::optional<std::string> &output_file, std::optional<unsigned long> &reasonable_number_of_temp_tapes);
};

#endif
