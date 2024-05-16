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
	unsigned pivot;
	long i = low - 1;
	tape->moveToBegin();
	tape->moveToIndex(low);
	unsigned pivotIndex = low + (high - low) / 2;
	for (long j = low; j <= high; ++j) {
		if (j != pivotIndex) {
			tape->moveForward();
			unsigned temp = tape->read();
			if (temp < pivot) {
				++i;
				tape->swap(i, j);
				tape->moveBack();
				tape->write(temp);
			}
		}
	}
	tape->moveToBegin();
	tape->moveToIndex(i + 1);
	for (long k = low; k <= i; ++k) {
		tape->moveForward();
	}
	tape->swap(i + 1, high);
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
	tape->moveToBegin();
	long tapeLength = 0;
	while (tape->canMoveForward()) {
		tape->moveForward();
		++tapeLength;
	}
	quicksort(tape, 0, tape->size()-1);
}

