# Chip-8 Emulator (C)

A lightweight Chip-8 emulator written in C using SDL2 for graphics and input. This project is an educational implementation of the Chip-8 virtual machine and includes a simple Makefile for building.

## Features

* Emulates the core Chip-8 instruction set
* Uses SDL2 for window rendering and keyboard input
* Configurable key mapping for the 16-key Chip-8 keypad
* Simple Makefile for easy compilation

## Requirements

* **C Compiler** (GCC or Clang recommended)
* **SDL2 Library** (development headers installed)

On macOS with Homebrew:

```bash
brew install sdl2
```

## Build Instructions

Clone or download the repository, then build with:

```bash
mkdir build bin && make
```

The compiled executable will be in `./bin/main`.

To clean build artifacts:

```bash
make clean
```

## Usage

Run the emulator:

```bash
./bin/main
```

Currently it opens a window and listens for key input. The Escape key quits the emulator. You can customize the key mappings in `config.h`.

## Project Structure

```bash
include/      Header files  
src/          Source code  
build/        Compiled object files  
bin/          Final executable  
Makefile      Build script  
```

## License

This project is open-source and available under the GPL-3.0 License.
