INCLUDES= -I./include -I/opt/homebrew/Cellar/sdl2/2.32.10/include
FLAGS= -g -Wall -Wextra
LIBS= -L/opt/homebrew/Cellar/sdl2/2.32.10/lib/ -lSDL2
OBJECTS= ./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o

all: ${OBJECTS}
	gcc ${FLAGS} ${LIBS} ${INCLUDES} ${OBJECTS} ./src/main.c -o ./bin/main

./build/chip8memory.o:./src/chip8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8memory.c -c -o ./build/chip8memory.o

./build/chip8stack.o:./src/chip8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/chip8keyboard.o:./src/chip8keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o

clean:
	rm -f ./build/*.o ./bin/main
