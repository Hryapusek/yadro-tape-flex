#include "ConfigReader.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>

#include "Exceptions.hpp"


InputCharacteristics ConfigReader::readFromFile(std::string filename)
{
  InputCharacteristics inputCharacteristics;
  std::ifstream inputFile(filename);

  std::optional<double> read_delay, write_delay, move_delay, move_one_pos_delay;

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

    double paramValue = std::stod(paramValueStr);

    if (paramName == "read_delay")
      read_delay = paramValue;
    else if (paramName == "write_delay")
      write_delay = paramValue;
    else if (paramName == "move_delay")
      move_delay = paramValue;
    else if (paramName == "move_one_pos_delay")
      move_one_pos_delay = paramValue;
    else
      std::cout << "Unused parameter " << paramName << " found in config file\n";
  }

  checkParameters(read_delay, write_delay, move_delay, move_one_pos_delay);
  inputCharacteristics.read_delay = *read_delay;
  inputCharacteristics.write_delay = *write_delay;
  inputCharacteristics.move_delay = *move_delay;
  inputCharacteristics.move_one_pos_delay = *move_one_pos_delay;

  inputFile.close();
  return inputCharacteristics;
}

void ConfigReader::checkParameters(std::optional<double> read_delay, std::optional<double> write_delay, 
                                   std::optional<double> move_delay, std::optional<double> move_one_pos_delay)
{
  if (not read_delay)
    throw BadConfigException("read_delay not found in config");
  if (not write_delay)
    throw BadConfigException("write_delay not found in config");
  if (not move_delay)
    throw BadConfigException("move_delay not found in config");
  if (not move_one_pos_delay)
    throw BadConfigException("move_one_pos_delay not found in config");
}

#define google_test
#ifdef google_test

#include <gtest/gtest.h>

TEST(config, default_config_read)
{
  std::filesystem::path path_to_config("test/config.txt");
  auto inputCharacteristics = ConfigReader::readFromFile(path_to_config.string());
  EXPECT_EQ(inputCharacteristics.read_delay, 1.25);
  EXPECT_EQ(inputCharacteristics.write_delay, 0.0005);
  EXPECT_EQ(inputCharacteristics.move_delay, 0.4);
  EXPECT_EQ(inputCharacteristics.move_one_pos_delay, 0.001);
}

#endif
