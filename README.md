# yadro-tape-flex
# Usage
On the start program tries to read in `config.txt` file in the current directory.

To use this program, follow these steps:
1. Configure the input and output file paths in the `config.txt` file.
2. Run the program and observe the sorting process.
3. Check the output file to view the sorted tape contents.

# Config Description

The `config.txt` file contains the following parameters:
- `input_file`: Path to the input tape file.
- `output_file`: Path to the output tape file.
- `read_delay_milliseconds`: Delay in milliseconds for read operations.
- `write_delay_milliseconds`: Delay in milliseconds for write operations.
- `move_delay_milliseconds`: Delay in milliseconds for move operations.
- `move_one_pos_delay_milliseconds`: Delay in milliseconds for moving one position.
- `reasonable_number_of_temp_tapes`: Maximum number of temporary tapes to use.

# Unit Tests

Unit tests are provided to ensure the correctness of the sorting algorithm. These tests cover scenarios with different memory constraints and verify the sorted output.

# Compiling with CMake
CMakeLists.txt contains option `option_test` - you can change it if you want to compile only program itself.

To compile the program with CMake, follow these steps:
1. Open a terminal.
2. Navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Generate the build files using CMake:
   ```
   cmake ..
   ```
   This will load all dependance libraries. Make sure you have compiler that supports `C++20` standard.
5. Build the program:
   ```
   cmake --build .
   ```

After these steps cmake will produce two files:
- tape_sorter
- tape_sorter_test

First is the original program. Second are unit tests for it. You can run it to make sure that program runs correctly. Program was tested on linux, all tests are passed.
## Program Architecture

The program architecture follows a modular design with components such as:
- `ConfigReader`: Responsible for reading and parsing configuration files.
- `TapeDevice`: Represents the tape devices used for input and output operations.
- `TapeSorter`: Contains the sorting algorithms and strategies.

The program utilizes CMake for building and includes unit tests using GoogleTest framework to ensure the correctness of the sorting algorithm.

### ConfigReader

class ConfigReader provides with static InputCharacteristics readFromFile(std::filesystem::path file) method. Reads fields and checks for bad values.

### TapeDevice

This package contains Interface ITapeDevice and file implementation of this interface FileTapeDevice.
FileTapeDevice has two constructors. One reads numbers from file and provides interface Tape to numbers array.
Another creates empty tape and then writes it to file in destructor.

Both constructors have input parameters, most of them about delays. Each method has delay that is implemented with `std::this_thread::sleep_for`.

### TapeSorter

TapeSorter provides with static void sort(ITapeDevice *inputTape, ITapeDevice *outputTape, const InputCharacteristics &characts) method.
Program logic depends on input tape length and available memory. So we have three strategies to work with:
- Case when `memory > input tape length`. In this case we just load tape into memory and sort it using internal facilities.
- Case when `memory < input tape length`. Here we create temp tapes, save numbers to them, sort them using `quicksort` in memory, write them to output sequentially, then sort output tape using `quicksort`.
- Case when `memory <<< input tape length`. Here we just write numbers to output tape then sort it right on the tape using quicksort.

Pattern strategy used to implement this task.
