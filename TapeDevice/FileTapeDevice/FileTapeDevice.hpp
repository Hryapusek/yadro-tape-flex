#ifndef FILE_TAPE_DEVICE_HPP
#define FILE_TAPE_DEVICE_HPP

#include "TapeDevice/Interface/ITapeDevice.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include <optional>
#include <fstream>


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
    /**
     * @brief Constructs a FileTapeDevice with specified characteristics.
     * @param characteristics The characteristics of the tape device.
     */
    FileTapeDevice(TapeDeviceCharacteristics characteristics);

    /**
     * @brief Constructs a FileTapeDevice with specified characteristics and size.
     * @param characteristics The characteristics of the tape device.
     * @param size The initial size of the tape.
     */
    FileTapeDevice(TapeDeviceCharacteristics characteristics, unsigned long size);

    /**
     * @brief Reads the value at the current position.
     * @return The value read.
     * @throw ReadFailedException if the read operation fails.
     */
    virtual unsigned read() const override;

    /**
     * @brief Writes a value at the current position.
     * @param value The value to write.
     * @throw WriteFailedException if the write operation fails.
     */
    virtual void write(unsigned value) override;

    /**
     * @brief Moves the tape position backwards.
     * @throw BeginReachedException if the beginning of the tape is reached.
     */
    virtual void moveBack() override;

    /**
     * @brief Moves the tape position forwards.
     * @throw EndReachedException if the end of the tape is reached and the tape is fixed size.
     */
    virtual void moveForward() override;

    /**
     * @brief Checks if the tape can move forward.
     * @return True if it can move forward, false otherwise.
     */
    virtual bool canMoveForward() const override;

    /**
     * @brief Checks if the tape can move backward.
     * @return True if it can move backward, false otherwise.
     */
    virtual bool canMoveBack() const override;

    /**
     * @brief Moves the tape position to the beginning.
     */
    virtual void moveToBegin() override;

    /**
     * @brief Moves the tape position to the end.
     * @throw std::runtime_error if called when isFixedSize_ is false.
     */
    virtual void moveToEnd() override;

    /**
     * @brief Moves the tape position to a specific index.
     * @param index The index to move to.
     * @throw EndReachedException if the index is out of bounds and the tape is fixed size.
     */
    virtual void moveToIndex(unsigned long index) override;

    /**
     * @brief Swaps the values at two positions.
     * @param lhs The left-hand side index.
     * @param rhs The right-hand side index.
     * @throw std::out_of_range if either index is out of bounds.
     */
    virtual void swap(unsigned long lhs, unsigned long rhs) override;

    /**
     * @brief Returns the size of the tape.
     * @return The size of the tape.
     */
    virtual unsigned long size() const override;

    /**
     * @brief Checks if the tape is empty.
     * @return True if the tape is empty, false otherwise.
     */
    virtual bool empty() override;

    /**
     * @brief Fixes the size of the tape, preventing further expansion.
     */
    virtual void fixSize() override;

    /**
     * @brief Converts the tape contents to a vector.
     * @return A vector containing the tape contents.
     */
    virtual std::vector<unsigned> toVector() override;

    /**
     * @brief Writes a vector of values to the tape.
     * @param arr The vector of values to write.
     * @throw WriteFailedException if the write operation fails.
     */
    virtual void write(std::vector<unsigned> arr) override;

    virtual bool hasValue() override;

    /**
     * @brief Destructor for FileTapeDevice.
     */
    virtual ~FileTapeDevice();

private:
    TapeDeviceCharacteristics characts_;
    std::vector<std::optional<unsigned>> tapeArray_;
    bool isFixedSize_ = false;
    std::vector<unsigned>::size_type position_ = 0;
    std::ofstream outputFile_;
};

#endif
