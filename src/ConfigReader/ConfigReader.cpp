#include "ConfigReader.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>
#include <algorithm>

#include "Exceptions.hpp"

namespace
{
	template <std::ctype_base::mask mask>
	class IsNot
	{
		std::locale myLocale;
		std::ctype<char> const* myCType;
	public:
		IsNot( std::locale const& l = std::locale() )
			: myLocale( l )
			, myCType( &std::use_facet<std::ctype<char> >( l ) )
		{
		}
		bool operator()( char ch ) const
		{
			return !myCType->is( mask, ch );
		}
	};

	typedef IsNot<std::ctype_base::space> IsNotSpace;

	std::string trim( std::string const& original )
	{
		std::string::const_iterator right = std::find_if( original.rbegin(), original.rend(), IsNotSpace() ).base();
		std::string::const_iterator left = std::find_if(original.begin(), right, IsNotSpace() );
		return std::string( left, right );
	}
}


InputCharacteristics ConfigReader::readFromFile(std::filesystem::path file)
{
	InputCharacteristics inputCharacteristics;
	std::ifstream inputFile(file);
	if (not inputFile.is_open())
		throw FileNotFoundException("File with name " + file.string() + " not found");

	std::optional<int> read_delay_milliseconds, write_delay_milliseconds, move_delay_milliseconds, move_one_pos_delay_milliseconds;
	std::optional<unsigned long> memory_bytes, reasonable_number_of_temp_tapes;
	std::optional<std::string> input_file, output_file;

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


		if (paramName == "read_delay_milliseconds")
			read_delay_milliseconds = std::stoi(paramValueStr);
		else if (paramName == "write_delay_milliseconds")
			write_delay_milliseconds = std::stoi(paramValueStr);
		else if (paramName == "move_delay_milliseconds")
			move_delay_milliseconds = std::stoi(paramValueStr);
		else if (paramName == "move_one_pos_delay_milliseconds")
			move_one_pos_delay_milliseconds = std::stoi(paramValueStr);
		else if (paramName == "memory_bytes")
			memory_bytes = std::stoul(paramValueStr);
		else if (paramName == "reasonable_number_of_temp_tapes")
			reasonable_number_of_temp_tapes = std::stoul(paramValueStr);
		else if (paramName == "input_file")
			input_file = paramValueStr;
		else if (paramName == "output_file")
			output_file = paramValueStr;
		else
			std::cout << "Unused parameter " << paramName << " found in config file\n";
	}

	checkParameters(read_delay_milliseconds, write_delay_milliseconds, move_delay_milliseconds, move_one_pos_delay_milliseconds, memory_bytes, input_file, output_file, reasonable_number_of_temp_tapes);
	
	inputCharacteristics.read_delay_milliseconds = *read_delay_milliseconds;
	inputCharacteristics.write_delay_milliseconds = *write_delay_milliseconds;
	inputCharacteristics.move_delay_milliseconds = *move_delay_milliseconds;
	inputCharacteristics.move_one_pos_delay_milliseconds = *move_one_pos_delay_milliseconds;
	inputCharacteristics.memory_bytes = *memory_bytes;
	inputCharacteristics.input_file = *input_file;
	inputCharacteristics.output_file = *output_file;
	inputCharacteristics.reasonable_number_of_temp_tapes = *reasonable_number_of_temp_tapes;

	inputFile.close();
	return inputCharacteristics;
}

void ConfigReader::checkParameters(std::optional<int> &read_delay_milliseconds, std::optional<int> &write_delay_milliseconds,
									std::optional<int> &move_delay_milliseconds, std::optional<int> &move_one_pos_delay_milliseconds,
									std::optional<unsigned long> &memory_bytes, std::optional<std::string> &input_file,
									std::optional<std::string> &output_file, std::optional<unsigned long> &reasonable_number_of_temp_tapes)
{
	if (not read_delay_milliseconds)
		throw FieldNotFoundException("read_delay_milliseconds not found in config");
	if (*read_delay_milliseconds <= 0)
		throw BadFieldException("read_delay_milliseconds less than or zero");

	if (not write_delay_milliseconds)
		throw FieldNotFoundException("write_delay_milliseconds not found in config");
	if (*write_delay_milliseconds <= 0)
		throw BadFieldException("write_delay_milliseconds less than or zero");

	if (not move_delay_milliseconds)
		throw FieldNotFoundException("move_delay_milliseconds not found in config");
	if (*move_delay_milliseconds <= 0)
		throw BadFieldException("move_delay_milliseconds less than or zero");

	if (not move_one_pos_delay_milliseconds)
		throw FieldNotFoundException("move_one_pos_delay_milliseconds not found in config");
	if (*move_one_pos_delay_milliseconds <= 0)
		throw BadFieldException("move_one_pos_delay_milliseconds less than or zero");

	if (not memory_bytes)
		throw FieldNotFoundException("memory_bytes not found in config");
	if (*memory_bytes <= 0)
		throw BadFieldException("memory_bytes less than or zero");

	if (not reasonable_number_of_temp_tapes)
		throw FieldNotFoundException("reasonable_number_of_temp_tapes not found in config");
	if (*reasonable_number_of_temp_tapes <= 0)
		throw BadFieldException("reasonable_number_of_temp_tapes less than or zero");

	if (not input_file)
		throw FieldNotFoundException("input_file not found in config");
	*input_file = trim(*input_file);
	if (input_file->size() == 0)
		throw BadFieldException("input_file less than or zero");

	if (not output_file)
		throw FieldNotFoundException("output_file not found in config");
	*output_file = trim(*output_file);
	if (output_file->size() == 0)
		throw BadFieldException("output_file less than or zero");
}


std::ostream &operator<<(std::ostream &out, const InputCharacteristics characts)
{
	out << "read_delay_milliseconds: " << characts.read_delay_milliseconds << '\n';
	out << "write_delay_milliseconds: " << characts.write_delay_milliseconds << '\n';
	out << "move_delay_milliseconds: " << characts.move_delay_milliseconds << '\n';
	out << "move_one_pos_delay_milliseconds: " << characts.move_one_pos_delay_milliseconds << '\n';
	out << "memory_bytes: " << characts.memory_bytes << '\n';
	out << "input_file: " << characts.input_file << '\n';
	out << "output_file: " << characts.output_file << '\n';
	out << "reasonable_number_of_temp_tapes: " << characts.reasonable_number_of_temp_tapes << '\n';
	return out;
}
