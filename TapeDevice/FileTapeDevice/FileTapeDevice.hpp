#ifndef FILE_TAPE_DEVICE_HPP
#define FILE_TAPE_DEVICE_HPP

#include "TapeDevice/Interface/ITapeDevice.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include <optional>


struct TapeDeviceCharacteristics
{
    std::filesystem::path tape_file;
    double read_delay;
    double write_delay;
    double move_delay;
    double move_one_pos_delay;
};

class FileTapeDevice: public ITapeDevice {

public:
    FileTapeDevice(TapeDeviceCharacteristics characteristics, bool readFromFile = false);
    virtual int read() const override;
    virtual void write(int value) override;
    virtual void moveBack() override;
    virtual void moveForward() override;
    virtual bool canMoveForward() const override;
    virtual bool canMoveBack() const override;
    virtual ~FileTapeDevice() = default;

private:
    std::vector<std::optional<int>> tapeArray;
    std::vector<int>::size_type position;
};

#endif
