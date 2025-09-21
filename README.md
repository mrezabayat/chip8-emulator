# Chip-8 Emulator (C)

A lightweight Chip-8 emulator written in C using SDL2 for graphics and input. This project is an educational implementation of the Chip-8 virtual machine and includes a simple Makefile for building.

![Demo of Chip-8 Emulator](docs/invador_demo.gif)

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

### ROMs (Git LFS)

Sample ROMs live under `roms/` and are tracked with [Git LFS](https://git-lfs.com/). Make sure Git LFS is installed before cloning:

```bash
brew install git-lfs   # macOS (Homebrew)
git lfs install
```

If you already cloned without LFS, pull the ROM binaries with:

```bash
git lfs pull
```

Once the ROMs are present, launch the emulator with one of them, for example:

```bash
./bin/main roms/INVADERS.ch8
```

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
