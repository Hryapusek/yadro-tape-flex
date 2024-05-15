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
};

std::ostream &operator<<(std::ostream &out, const InputCharacteristics characts);


class ConfigReader
{
public:
    static InputCharacteristics readFromFile(std::filesystem::path file);

private:
    static void checkParameters(std::optional<double> read_delay, std::optional<double> write_delay, 
                                std::optional<double> move_delay, std::optional<double> move_one_pos_delay,
                                std::optional<unsigned long> memory_bytes);
};

#endif
