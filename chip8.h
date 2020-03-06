#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define true 1
#define false 0
#define bool int

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

uint16_t pc;
uint16_t opcode;
uint16_t I;
uint16_t sp;

uint8_t registers[16];
uint8_t memory[MEM_SIZE];
uint16_t stack[STACK_SIZE];

uint8_t delay_timer;
uint8_t sound_timer;

uint8_t draw_flag;

uint8_t key[KEY_SIZE];
uint8_t gfx[GFX_SIZE];

void chip8_init();
void chip8_loadgame(char* filename);
void chip8_emulatecycle();
void chip8_setkeys();
void chip8_tick();
void print_state();
void print_memory();

#endif
