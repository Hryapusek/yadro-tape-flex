#ifndef ITAPE_DEVICE_HPP
#define ITAPE_DEVICE_HPP

#include <string>

class ITapeDevice {
public:
    virtual int read() const = 0;
    virtual void write(int value) = 0;
    virtual void moveBack() = 0;
    virtual void moveForward() = 0;
    virtual bool canMoveForward() const = 0;
    virtual bool canMoveBack() const = 0;
    virtual ~ITapeDevice() = default;
};

#endif
