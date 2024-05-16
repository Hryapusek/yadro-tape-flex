#ifndef ITAPE_DEVICE_HPP
#define ITAPE_DEVICE_HPP

#include "Exceptions.hpp"
#include <string>
#include <vector>

class ITapeDevice {
public:
    /**
     * @throw ReadFailedException if reading fails
     */
    virtual unsigned read() const = 0;

    /**
     * @throw WriteFailedException if writing fails
     */
    virtual void write(unsigned value) = 0;

    /**
     * @throw BeginReachedException if moving back reaches the beginning
     */
    virtual void moveBack() = 0;

    /**
     * @throw EndReachedException if moving forward reaches the end
     */
    virtual void moveForward() = 0;
    virtual bool canMoveForward() const = 0;
    virtual bool canMoveBack() const = 0;
    virtual void moveToBegin() = 0;
    virtual void moveToEnd() = 0;
    virtual void moveToIndex(unsigned long index) = 0;
    virtual bool empty() = 0;
    virtual void swap(unsigned long lhs, unsigned long rhs) = 0;
    virtual bool hasValue() = 0;
    virtual unsigned long size() const = 0;
    virtual std::vector<unsigned> toVector() = 0;

    /**
     * @throw WriteFailedException if writing the vector fails
     */
    virtual void write(std::vector<unsigned> arr) = 0;
    virtual ~ITapeDevice() = default;
};

#endif
