# CHIP-8 emulator

Chip-8 is a simple, interpreted, assembly language which was first used on some do-it-yourself computer systems in the late 1970s and early 1980s. The COSMAC VIP, DREAM 6800, and ETI 660 computers are a few examples. These computers typically were designed to use a television as a display, had between 1 and 4K of RAM, and used a 16-key hexadecimal keypad for input. The interpreter took up only 512 bytes of memory, and programs, which were entered into the computer in hexadecimal, were even smaller.

## TODO
- Refactor
-  Fix Sound

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

To build the chip-8 emulator you will need to have OpenGL and GLUT installed.
Ubuntu example:
```
sudo apt-get install libglut3-dev
```

### Building

Run```make```and then call ```./play <rom>``` with your rom of choice. Several sample roms are given in the test_files folder.

## References
- [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [Chip-8 Wikipedia page](https://en.wikipedia.org/wiki/CHIP-8)
