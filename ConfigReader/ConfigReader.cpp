#include "ConfigReader.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>

#include "Exceptions.hpp"

/**
 * @throw - FieldNotFoundException if any necessary field was not found
 * @throw - FileNotFoundException
 * @throw - BadFieldException
 * @throw - std::exception if conversion to number fail
 */
InputCharacteristics ConfigReader::readFromFile(std::filesystem::path file)
{
	InputCharacteristics inputCharacteristics;
	std::ifstream inputFile(file);
	if (not inputFile.is_open())
		throw FileNotFoundException("File with name " + file.string() + " not found");

	std::optional<double> read_delay, write_delay, move_delay, move_one_pos_delay;
	std::optional<unsigned long> memory_bytes;

	std::string line;

	while (std::getline(inputFile, line))
	{
		size_t colonPos = line.find(':');

		if (colonPos == std::string::npos)
		{
			std::cout << "Trash in the config file. Skipping line\n";
			continue;
		}
		std::string paramName = line.substr(0, colonPos);

		paramName.erase(0, paramName.find_first_not_of(" \t"));
		paramName.erase(paramName.find_last_not_of(" \t") + 1);

		std::string paramValueStr = line.substr(colonPos + 1);


		if (paramName == "read_delay")
			read_delay = std::stod(paramValueStr);
		else if (paramName == "write_delay")
			write_delay = std::stod(paramValueStr);
		else if (paramName == "move_delay")
			move_delay = std::stod(paramValueStr);
		else if (paramName == "move_one_pos_delay")
			move_one_pos_delay = std::stod(paramValueStr);
    else if (paramName == "memory_bytes")
      memory_bytes = std::stoul(paramValueStr);
		else
			std::cout << "Unused parameter " << paramName << " found in config file\n";
	}

	checkParameters(read_delay, write_delay, move_delay, move_one_pos_delay, memory_bytes);
	inputCharacteristics.read_delay = *read_delay;
	inputCharacteristics.write_delay = *write_delay;
	inputCharacteristics.move_delay = *move_delay;
	inputCharacteristics.move_one_pos_delay = *move_one_pos_delay;
	inputCharacteristics.memory_bytes = *memory_bytes;

	inputFile.close();
	return inputCharacteristics;
}

void ConfigReader::checkParameters(std::optional<double> read_delay, std::optional<double> write_delay,
                                   std::optional<double> move_delay, std::optional<double> move_one_pos_delay,
								   std::optional<unsigned long> memory_bytes)
{
	if (not read_delay)
		throw FieldNotFoundException("read_delay not found in config");
	if (*read_delay <= 0)
		throw BadFieldException("read_delay less than or zero");

	if (not write_delay)
		throw FieldNotFoundException("write_delay not found in config");
	if (*write_delay <= 0)
		throw BadFieldException("write_delay less than or zero");

	if (not move_delay)
		throw FieldNotFoundException("move_delay not found in config");
	if (*move_delay <= 0)
		throw BadFieldException("move_delay less than or zero");

	if (not move_one_pos_delay)
		throw FieldNotFoundException("move_one_pos_delay not found in config");
	if (*move_one_pos_delay <= 0)
		throw BadFieldException("move_one_pos_delay less than or zero");
		
	if (not memory_bytes)
		throw FieldNotFoundException("memory_bytes not found in config");
	if (*memory_bytes <= 0)
		throw BadFieldException("memory_bytes less than or zero");
}


std::ostream &operator<<(std::ostream &out, const InputCharacteristics characts)
{
    out << "read_delay: " << characts.read_delay << '\n';
	out << "write_delay: " << characts.write_delay << '\n';
	out << "move_delay: " << characts.move_delay << '\n';
	out << "move_one_pos_delay: " << characts.move_one_pos_delay << '\n';
	out << "memory_bytes: " << characts.memory_bytes << '\n';
	return out;
}
