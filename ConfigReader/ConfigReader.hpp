#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <string>
#include <optional>

struct InputCharacteristics
{
    double read_delay;
    double write_delay;
    double move_delay;
    double move_one_pos_delay;
};


class ConfigReader
{
public:
    static InputCharacteristics readFromFile(std::string filename);

private:
    static void checkParameters(std::optional<double> read_delay, std::optional<double> write_delay, 
                                std::optional<double> move_delay, std::optional<double> move_one_pos_delay);
};

#endif
