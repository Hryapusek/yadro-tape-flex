#include "FileTapeDevice.hpp"
#include <cassert>
#include <algorithm>
#include <thread>
#include <iostream>

FileTapeDevice::FileTapeDevice(TapeDeviceCharacteristics characteristics, unsigned long size) :
    characts_(std::move(characteristics)),
    tapeArray_(size, std::nullopt),
    position_(0),
    outputFile_(characts_.tape_file)
{}

FileTapeDevice::FileTapeDevice(TapeDeviceCharacteristics characteristics) :
    characts_(std::move(characteristics)),
    position_(0)
{
    std::ifstream file(characts_.tape_file);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line, ' '))
        {
            unsigned number = std::stoul(line);
            tapeArray_.push_back(number);
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("File could not be opened: " + characts_.tape_file.string());
    }
}

unsigned FileTapeDevice::read() const
{
    if (!tapeArray_[position_].has_value())
    {
        throw ReadFailedException("Attempted to read an uninitialized position.");
    }
    std::cout << "File: " << characts_.tape_file << ", Operation: read, Position: " << position_ << ", Delay: " << characts_.read_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.read_delay_milliseconds));
    return tapeArray_[position_].value();
}

void FileTapeDevice::write(unsigned value)
{
    if (position_ >= tapeArray_.size()) {
        throw WriteFailedException("Attempted to write beyond the current tape size.");
    }
    std::cout << "File: " << characts_.tape_file << ", Operation: write, Position: " << position_ << ", Delay: " << characts_.write_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.write_delay_milliseconds));
    tapeArray_[position_] = value;
}

void FileTapeDevice::moveBack()
{
    if (tapeArray_.size() == 0)
        return;
    if (position_ == 0)
    {
        throw BeginReachedException("Beginning of tape reached.");
    }
    std::cout << "File: " << characts_.tape_file << ", Operation: moveBack, Position: " << position_ << ", Delay: " << characts_.move_one_pos_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.move_one_pos_delay_milliseconds));
    --position_;
}

void FileTapeDevice::moveForward()
{
    if (tapeArray_.size() == 0)
        return;
    if (position_ >= tapeArray_.size() - 1)
    {
        throw EndReachedException("End of tape reached.");
    }
    std::cout << "File: " << characts_.tape_file << ", Operation: moveForward, Position: " << position_ << ", Delay: " << characts_.move_one_pos_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.move_one_pos_delay_milliseconds));
    ++position_;
}

bool FileTapeDevice::canMoveForward() const
{
    return tapeArray_.size() != 0 and position_ < tapeArray_.size() - 1;
}
bool FileTapeDevice::canMoveBack() const
{
    return position_ > 0;
}

void FileTapeDevice::moveToBegin()
{
    if (tapeArray_.size() == 0)
        return;
    std::cout << "File: " << characts_.tape_file << ", Operation: moveToBegin, Position: " << position_ << ", Delay: " << characts_.move_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.move_delay_milliseconds));
    position_ = 0;
}

void FileTapeDevice::moveToEnd()
{
    if (tapeArray_.size() == 0)
        return;
    std::cout << "File: " << characts_.tape_file << ", Operation: moveToEnd, Position: " << position_ << ", Delay: " << characts_.move_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.move_delay_milliseconds));
    position_ = tapeArray_.size() - 1;
}

void FileTapeDevice::moveToIndex(unsigned long index)
{
    if (index >= tapeArray_.size()) {
        throw EndReachedException("Index out of bounds.");
    }
    std::cout << "File: " << characts_.tape_file << ", Operation: moveToIndex, Position: " << position_ << ", Delay: " << characts_.move_delay_milliseconds << "ms\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(characts_.move_delay_milliseconds));
    position_ = index;
}

void FileTapeDevice::swap(unsigned long lhs, unsigned long rhs)
{
    if (lhs >= tapeArray_.size() || rhs >= tapeArray_.size()) {
        throw std::out_of_range("Index out of bounds for swap operation.");
    }
    if (lhs == rhs) return;

    moveToIndex(lhs);
    unsigned lhsValue = read();

    moveToIndex(rhs);
    unsigned rhsValue = read();
    write(lhsValue);

    moveToIndex(lhs);
    write(rhsValue);
}

unsigned long FileTapeDevice::size() const
{
    return tapeArray_.size();
}

bool FileTapeDevice::empty()
{
    return tapeArray_.empty() || std::all_of(tapeArray_.begin(), tapeArray_.end(), [](const std::optional<unsigned>& elem) { return !elem.has_value(); });
}

std::vector<unsigned> FileTapeDevice::toVector()
{
    std::vector<unsigned> result;
    if (!empty()) {
        moveToBegin();
        do {
            if (tapeArray_[position_].has_value()) {
                result.push_back(tapeArray_[position_].value());
            }
        } while (canMoveForward() && (moveForward(), true));
    }
    return result;
}

void FileTapeDevice::write(std::vector<unsigned> arr)
{
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        write(*it);
        if (canMoveForward()) {
            moveForward();
        } else {
            if (std::next(it) != arr.end())
                throw std::out_of_range("Reached end of tape while writing.");
        }
    }
}

bool FileTapeDevice::hasValue()
{
    return not empty() and tapeArray_[position_].has_value();
}

FileTapeDevice::~FileTapeDevice() {
    if (outputFile_.is_open()) {
        for (const auto& value : tapeArray_) {
            if (value.has_value()) {
                outputFile_ << value.value() << " ";
            }
        }
        outputFile_.close();
    }
}

