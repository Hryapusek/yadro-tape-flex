#include "Utils.hpp"

unsigned long TapeSorterUtils::calculateMaxArrayLength(const InputCharacteristics & characts)
{
	return characts.memory_bytes / 4;
}

bool TapeSorterUtils::isTapeEmpty(ITapeDevice * inputTape)
{
	if (not inputTape->canMoveBack())
	{
		try
		{
			inputTape->read();
			return false;
		}
		catch(const ReadFailedException& e)
		{
			return true;
		}

	}
	return false;
}

long partition(ITapeDevice *tape, long low, long high) {
    long i = low - 1;
    unsigned pivotIndex = low + (high - low) / 2;
    tape->moveToIndex(pivotIndex);
    unsigned pivot = tape->read();  // Read the pivot value at the pivotIndex
    tape->swap(pivotIndex, high);  // Move pivot to end for easier loop handling
    for (long j = low; j < high; ++j) {
        tape->moveToIndex(j);
        unsigned temp = tape->read();
        if (temp < pivot) {
            ++i;
            tape->moveToIndex(i);
            tape->swap(i, j);
        }
    }
    tape->moveToIndex(i + 1);
    tape->swap(i + 1, high);  // Move pivot to its final place
    return i + 1;
}

void quicksort(ITapeDevice *tape, long start, long end) {
    if (start < end) {
        long pivot = partition(tape, start, end);
        quicksort(tape, start, pivot - 1);
        quicksort(tape, pivot + 1, end);
    }
}

void TapeSorterUtils::sortTape(ITapeDevice *tape) {
    if (tape->empty()) return;  // Early exit if tape is empty

    tape->moveToBegin();
    long lastValuedIndex = -1;
    for (long i = 0; i < tape->size(); ++i) {
        if (tape->hasValue()) {
            lastValuedIndex = i;
            if (i < tape->size() - 1)
                tape->moveForward();
        }
        else break;
    }
    if (lastValuedIndex != -1) {
        quicksort(tape, 0, lastValuedIndex);
    }
}
