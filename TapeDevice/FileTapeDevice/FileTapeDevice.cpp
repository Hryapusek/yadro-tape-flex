#include "FileTapeDevice.hpp"
#include <cassert>
#include <algorithm>

FileTapeDevice::FileTapeDevice(TapeDeviceCharacteristics characteristics, unsigned long size) :
    characts_(std::move(characteristics)),
    tapeArray_(size, std::nullopt),
    isFixedSize_(true),
    position_(0),
    outputFile_(characts_.tape_file)
{}

FileTapeDevice::FileTapeDevice(TapeDeviceCharacteristics characteristics) :
    characts_(std::move(characteristics)),
    isFixedSize_(true),
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
    return tapeArray_[position_].value();
}

void FileTapeDevice::write(unsigned value)
{
    if (position_ >= tapeArray_.size()) {
        throw WriteFailedException("Attempted to write beyond the current tape size.");
    }
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
    --position_;
}

void FileTapeDevice::moveForward()
{
    if (tapeArray_.size() == 0)
        return;
    if (position_ >= tapeArray_.size() - 1)
    {
        if (!isFixedSize_) {
            tapeArray_.push_back(std::nullopt); // Expand the tape with an uninitialized value
        } else {
            throw EndReachedException("End of tape reached.");
        }
    }
    ++position_;
}

bool FileTapeDevice::canMoveForward() const
{
    if (isFixedSize_) {
        return tapeArray_.size() != 0 and position_ < tapeArray_.size() - 1;
    } else {
        return true; // Can always move forward if the tape is not fixed size, as it can expand
    }
}
bool FileTapeDevice::canMoveBack() const
{
    return position_ > 0;
}

void FileTapeDevice::moveToBegin()
{
    if (tapeArray_.size() == 0)
        return;
    while (position_ > 0) {
        moveBack();
    }
}

void FileTapeDevice::moveToEnd()
{
    if (tapeArray_.size() == 0)
        return;
    if (!isFixedSize_) {
        while (position_ < tapeArray_.size() - 1) {
            moveForward();
        }
    } else {
        throw std::runtime_error("moveToEnd called with isFixedSize_ set to false");
    }
}

void FileTapeDevice::moveToIndex(unsigned long index)
{
    if (index >= tapeArray_.size()) {
        if (isFixedSize_) {
            throw EndReachedException("Index out of bounds.");
        } else {
            while (tapeArray_.size() <= index) {
                tapeArray_.push_back(std::nullopt); // Expand the tape with an uninitialized value
            }
        }
    }
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

void FileTapeDevice::fixSize()
{
    assert(!isFixedSize_);
    isFixedSize_ = true;
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
    for (unsigned value : arr) {
        write(value);
        if (canMoveForward()) {
            moveForward();
        } else {
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

