#include "chip8.h"

#define unknown_opcode(X) printf("Unknown opcode 0x%x\n", X)

static inline uint8_t randbyte() {return rand() % 256;}

unsigned char chip8_fontset[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0,
  0x20, 0x60, 0x20, 0x20, 0x70,
  0xF0, 0x10, 0xF0, 0x80, 0xF0,
  0xF0, 0x10, 0xF0, 0x10, 0xF0,
  0x90, 0x90, 0xF0, 0x10, 0x10,
  0xF0, 0x80, 0xF0, 0x10, 0xF0,
  0xF0, 0x80, 0xF0, 0x90, 0xF0,
  0xF0, 0x10, 0x20, 0x40, 0x40,
  0xF0, 0x90, 0xF0, 0x90, 0xF0,
  0xF0, 0x90, 0xF0, 0x10, 0xF0,
  0xF0, 0x90, 0xF0, 0x90, 0x90,
  0xE0, 0x90, 0xE0, 0x90, 0xE0,
  0xF0, 0x80, 0x80, 0x80, 0xF0,
  0xE0, 0x90, 0x90, 0x90, 0xE0,
  0xF0, 0x80, 0xF0, 0x80, 0xF0,
  0xF0, 0x80, 0xF0, 0x80, 0x80
};

void draw_sprite(uint8_t x, uint8_t y, uint8_t h) {
//  int xline, yline;
//  uint8_t pixel;
//
//  registers[VF] = 0;
//  for (yline = 0; yline < h; yline++) {
//    pixel = memory[I + yline];
//    for (xline = 0; xline < 8; xline++) {
//      if (pixel & (0x80 >> xline)) {
//        if (gfx[x + xline + 64*(y + yline)] == 1) {
//          registers[VF] = 1;
//        }
//        gfx[x + xline + 64*(y + yline)] ^= 0x1;
//      }
//    }
//  }
    unsigned row = y, col = x;
    unsigned byte_index;
    unsigned bit_index;

    // set the collision flag to 0
    registers[0xF] = 0;
    for (byte_index = 0; byte_index < h; byte_index++) {
        uint8_t byte = memory[I + byte_index];

        for (bit_index = 0; bit_index < 8; bit_index++) {
            // the value of the bit in the sprite
            uint8_t bit = (byte >> bit_index) & 0x1;
            // the value of the current pixel on the screen
            uint8_t *pixelp = &gfx[(row + byte_index) % GFX_ROWS]
                                  [(col + (7 - bit_index)) % GFX_COLS];

            // if drawing to the screen would cause any pixel to be erased,
            // set the collision flag to 1
            if (bit == 1 && *pixelp ==1) registers[0xF] = 1;

            // draw this pixel by XOR
            *pixelp = *pixelp ^ bit;
        }
    }
}


void chip8_init() {

  int i;

  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  for (i = 0; i < 80; i++)
    memory[i] = chip8_fontset[i];

  delay_timer = 0;
  sound_timer = 0;

  draw_flag = true;

  srand(time(NULL));
}


void chip8_loadgame(char* filename) {

  FILE* rom = fopen(filename, "rb");

  fread(&memory[0x200], 1, MAX_GAME_SIZE, rom);

  fclose(rom);

}


void chip8_emulatecycle() {

  int i;
  uint8_t x, y, n, kk;
  uint16_t nnn;

  opcode = (memory[pc] << 8) | memory[pc + 1];
  x = (opcode >> 8) & 0x000F;
  y = (opcode >> 4) & 0x000F;
  n = opcode & 0x000F;
  kk = opcode & 0x00FF;
  nnn = opcode & 0x0FFF;

  switch (opcode & 0xF000) {

    case 0x0000:
      switch (kk) {
        case 0xE0:
          printf("Clear the screen\n");
          memset(gfx, 0, sizeof(uint8_t) * GFX_SIZE);
          draw_flag = true;
          pc += 2;
          break;
        case 0xEE:
          printf("Return from subroutine call\n");
          sp--;
          pc = stack[sp];
          break;
       default:
          unknown_opcode(opcode);
      }
      break;

    case 0x1000:
      printf("Jump to address 0x%x\n", nnn);
      pc = nnn;
      break;

    case 0x2000:
      printf("Jump to subroutine at address 0x%x\n", nnn);
      stack[sp] = pc + 2;
      sp++;
      pc = nnn;
      break;

    case 0x3000:
      printf("Skip next instruction if V%x == 0x%x\n", x, kk);
      pc += registers[x] == kk ? 4 : 2;
      break;

    case 0x4000:
      printf("Skip next instruction if V%x != 0x%x\n", x, kk);
      pc += registers[x] != kk ? 4 : 2;
      break;

    case 0x5000:
      printf("Skip next instruction if V%x == V%x\n", x, y);
      pc += registers[x] == registers[y] ? 4 : 2;
      break;

    case 0x6000:
      printf("Move 0x%x to V%x\n", kk, x);
      registers[x] = kk;
      pc += 2;
      break;

    case 0x7000:
      printf("Add 0x%x to V%x\n", kk, x);
      registers[x] += kk;
      pc += 2;
      break;

    case 0x8000:
      switch (n) {
        case 0x0:
          printf("Move V%x to V%x\n", y, x);
          registers[x] = registers[y];
          break;
        case 0x1:
          printf("V%x = V%x | V%x\n", x, x, y);
          registers[x] = registers[x] | registers[y];
          break;
        case 0x2:
          printf("V%x = V%x & V%x\n", x, x, y);
          registers[x] = registers[x] & registers[y];
          break;
        case 0x3:
          printf("V%x = V%x ^ V%x\n", x, x, y);
          registers[x] = registers[x] ^ registers[y];
          break;
        case 0x4:
          printf("V%x = V%x + V%x\n", x, x, y);
          registers[x] = registers[x] + registers[y];
          registers[VF] = ((int) registers[x] + (int) registers[y]) > 255 ? 1 : 0;
          break;
        case 0x5:
          printf("V%x = V%x - V%x\n", x, x, y);
          registers[x] = registers[x] - registers[y];
          registers[VF] = registers[x] > registers[y] ? 1 : 0;
          break;
        case 0x6:
          printf("V%x = V%x >> 1\n", x, x);
          registers[x] = registers[x] >> 1;
          registers[VF] = registers[x] & 0x1;
          break;
        case 0x7:
          printf("V%x = V%x - V%x\n", x, y, x);
          registers[x] = registers[y] - registers[x];
          registers[VF] = registers[y] > registers[x] ? 1 : 0;
          break;
        case 0xE:
          printf("V%x = V%x << 1\n", x, x);
          registers[VF] = (registers[x] >> 7) & 0x1;
          registers[x] = registers[x] << 1;
          break;
        default:
          unknown_opcode(opcode);
      }
      pc += 2;
      break;

    case 0x9000:
      switch (n) {
        case 0x0:
          printf("Skip next instruction if V%x != V%x\n", x, y);
          pc += registers[x] != registers[y] ? 4 : 2;
          break;
        default:
          unknown_opcode(opcode);
      }
      break;

    case 0xA000:
      printf("Load I with constant NNN: 0x%x\n", nnn);
      I = nnn;
      pc += 2;
      break;

    case 0xB000:
      printf("Jump to 0x%x + V[0] (0x%x)\n", nnn, registers[V0]);
      pc = nnn + registers[V0];
      break;

    case 0xC000:
      printf("register V%x = randbyte() & %x\n", x, kk);
      registers[x] = randbyte() & kk;
      pc += 2;
      break;

    case 0xD000:
      printf("Draw sprite at (V%x, V%x) with height %x\n", x, y, n);
      draw_sprite(registers[x], registers[y], n);
      pc += 2;
      draw_flag = true;
      break;

    case 0xE000:
      switch (kk) {
        case 0x9E:
          printf("if key[%x] is pressed skip next insturction\n", x);
          pc += key[x] ? 4 : 2;
          break;
        case 0xA1:
          printf("if key[%x] is pressed skip next insturction\n", x);
          pc += !key[x] ? 4 : 2;
          break;
      }
      break;
    case 0xF000:
      switch (kk) {
        case 0x07:
          printf("V%x = delay_timer\n", x);
          registers[x] = delay_timer;
          pc += 2;
          break;
        case 0x0A:
          printf("Wait for key instruction\n");
          for (;;) {
            for (i = 0; i < KEY_SIZE; i++) {
              if (key[i]) {
                registers[x] = i;
                goto key_pressed;
              }
            }
          }
          key_pressed:
          pc += 2;
          break;
        case 0x15:
          printf("delay_timer = V%x\n", x);
          delay_timer = registers[x];
          pc += 2;
          break;
        case 0x18:
          printf("sound_timer = V%x\n", x);
          sound_timer = registers[x];
          pc += 2;
          break;
        case 0x1E:
          printf("I += V%x\n", x);
          I += registers[x];
          pc += 2;
          break;
        case 0x29:
          printf("I = lovation of font for charavter V%x\n", x);
          I = 5 * registers[x];
          pc += 2;
          break;
        case 0x33:
          printf("I = bcd repr. of V%x\n", x);
          memory[I]   = (registers[x] % 1000) / 100;
          memory[I+1] = (registers[x] % 100) / 10;
          memory[I+2] = (registers[x] % 10);
          pc += 2;
          break;
        case 0x55:
          printf("Copy sprite from registers V0 to V%x into memory at address 0x%x\n", x, I);
          for (i = 0; i <= x; i++) {
            memory[I + 1] = registers[i];
          }
          I += x + 1;
          pc += 2;
          break;
        case 0x65:
          printf("Copy sprite from memory at address 0x%x into registers 0 to 0x%x\n", x, I);
          for (i = 0; i <= x; i++) {
            registers[i] = memory[I + 1];
          }
          I += x + 1;
          pc += 2;
          break;
        default:
          unknown_opcode(opcode);
      }
      break;
    default:
      unknown_opcode(opcode);
  }

  //chip8_tick();
}

void chip8_tick() {
  if (delay_timer > 0)
    delay_timer--;
  if (sound_timer > 0) {
    sound_timer--;
    if (sound_timer == 0)
      printf("Sound Timer == 0!");
  }
}

void print_state() {
  printf("chip8 state:\npc: %d\nopcode: %d\nI: %d\nsp: %d\n", pc, opcode, I, sp);
}

void print_memory() {
  for (int i = 0; i < MEM_SIZE; i++) {
    printf("%X\n", memory[i]);
  }
}
