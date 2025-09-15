UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  CC ?= clang
else
  CC ?= $(CC)
endif

INCLUDES= -I./include -I/opt/homebrew/Cellar/sdl2/2.32.10/include
FLAGS= -g -Wall -Wextra 
LIBS= -L/opt/homebrew/Cellar/sdl2/2.32.10/lib/ -lSDL2

FLAGS = -g -Wall -Wextra
LIBS  = -L/opt/homebrew/Cellar/sdl2/2.32.10/lib/ -lSDL2

ifeq ($(UNAME_S),Darwin)
  FLAGS += -fblocks
  LIBS  += -framework AudioToolbox -framework AudioUnit -framework CoreAudio
endif

OBJECTS= ./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o ./build/chip8.o ./build/chip8screen.o

all: ${OBJECTS}
	$(CC) ${FLAGS} ${LIBS} ${INCLUDES} ${OBJECTS} ./src/main.c -o ./bin/main

./build/chip8memory.o:./src/chip8memory.c
	$(CC) ${FLAGS} ${INCLUDES} ./src/chip8memory.c -c -o ./build/chip8memory.o

./build/chip8stack.o:./src/chip8stack.c
	$(CC) ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/chip8keyboard.o:./src/chip8keyboard.c
	$(CC) ${FLAGS} ${INCLUDES} ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o

./build/chip8.o:./src/chip8.c
	$(CC) ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/chip8screen.o:./src/chip8screen.c
	$(CC) ${FLAGS} ${INCLUDES} ./src/chip8screen.c -c -o ./build/chip8screen.o

clean:
	rm -f ./build/*.o ./bin/main
