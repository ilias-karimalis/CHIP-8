play: chip8.c main.c
	gcc -o play chip8.c main.c -lGL -lglut -lGLU
