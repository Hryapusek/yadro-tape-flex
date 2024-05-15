#include "FileTapeDevice.hpp"

FileTapeDevice::FileTapeDevice(TapeDeviceCharacteristics characteristics, bool readFromFile)
{
}

int FileTapeDevice::read() const
{
    return 0;
}

void FileTapeDevice::write(int value)
{
}

void FileTapeDevice::moveBack()
{
}

void FileTapeDevice::moveForward()
{
}

bool FileTapeDevice::canMoveForward() const
{
    return false;
}

bool FileTapeDevice::canMoveBack() const
{
    return false;
}
