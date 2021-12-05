all: chip8.c main.c
	gcc -o chip8 chip8.c main.c -lGL -lglut -lGLU
