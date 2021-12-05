#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;

#define MEM_SIZE 0x1000
#define STACK_SIZE 16
#define KEY_SIZE 16

#define GFX_ROWS 32
#define GFX_COLS 64
#define GFX_SIZE (GFX_ROWS * GFX_COLS)

#define MAX_GAME_SIZE (MEM_SIZE - 0x200)

enum registers {
  V0 = 0x0,
  V1 = 0x1,
  V2 = 0x2,
  V3 = 0x3,
  V4 = 0x4,
  V5 = 0x5,
  V6 = 0x6,
  V7 = 0x7,
  V8 = 0x8,
  V9 = 0x9,
  VA = 0xa,
  VB = 0xb,
  VC = 0xc,
  VD = 0xd,
  VE = 0xe,
  VF = 0xf
};

u16 pc;
u16 opcode;
u16 I;
u16 sp;

u8 registers[16];
u8 memory[MEM_SIZE];
u16 stack[STACK_SIZE];

u8 delay_timer;
u8 sound_timer;

u8 draw_flag;

u8 key[KEY_SIZE];
u8 gfx[GFX_ROWS][GFX_COLS];

void Initialize();
void LoadGame(char* filename);
void EmulateCycle();
void chip8_setkeys();
void chip8_tick();
void print_state();
void print_memory();

#endif
