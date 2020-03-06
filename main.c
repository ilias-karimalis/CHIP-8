#include "chip8.h"

#include <stdio.h>
#include <GL/freeglut.h>
#include <sys/time.h>

#define CLOCK_HZ 60
#define CLOCK_RATE_MS ((int) ((1.0/ CLOCK_HZ) * 1000 + 0.5))

enum colors {
  Black = 0,
  White = 255
};

#define PIXEL_SIZE 10
#define SCREEN_ROWS  (GFX_ROWS * PIXEL_SIZE)
#define SCREEN_COLS  (GFX_COLS * PIXEL_SIZE)

unsigned char screen[GFX_ROWS][GFX_COLS][3];

struct timeval clock_prev;

void chip8_keydown(unsigned char k, int x, int y);
void chip8_keyup(unsigned char k, int x, int y);
void paint_pixel(int x, int y, unsigned char color);
void paint_cell(int x, int y, unsigned char color);
void reshape_window(GLsizei w, GLsizei h);
int timediff_ms(struct timeval* end, struct timeval* start);
void gfx_setup();

void gfx_setup() {
    memset(screen, Black, sizeof(unsigned char) * SCREEN_ROWS * SCREEN_COLS * 3);
    glClear(GL_COLOR_BUFFER_BIT);
}

void reshape_window(GLsizei w, GLsizei h) {
  (void) w; (void) h;
}

void paint_pixel(int x, int y, unsigned char color) {
    x = SCREEN_ROWS - 1 - x;
    screen[x][y][0] = screen[x][y][1] = screen[x][y][2] = color;
}

int timediff_ms(struct timeval *end, struct timeval *start) {
    int diff =  (end->tv_sec - start->tv_sec) * 1000 +
                (end->tv_usec - start->tv_usec) / 1000;
    //printf("timediff = %d\n", diff);
    return diff;
}

void paint_cell(int row, int col, unsigned char color) {
    int pixel_row = row * PIXEL_SIZE;
    int pixel_col = col * PIXEL_SIZE;
    int drow, dcol;

    for (drow = 0; drow < PIXEL_SIZE; drow++) {
        for (dcol = 0; dcol < PIXEL_SIZE; dcol++) {
            paint_pixel(pixel_row + drow, pixel_col + dcol, color);
        }
    }
}

void draw() {
    int row, col;

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw pixels to the buffer
    for (row = 0; row < GFX_ROWS; row++) {
        for (col = 0; col < GFX_COLS; col++) {
            paint_cell(row, col, gfx[row*col] ? White : Black);
        }
    }

    // Update Texture
    glDrawPixels(SCREEN_COLS, SCREEN_ROWS, GL_RGB, GL_UNSIGNED_BYTE, 
                 (void *) screen);
    glutSwapBuffers(); 
}

void loop() {
    struct timeval clock_now;
    gettimeofday(&clock_now, NULL);

    chip8_emulatecycle();

    if (draw_flag) {
        draw();
        draw_flag = false;
    }

    if (timediff_ms(&clock_now, &clock_prev) >= CLOCK_RATE_MS) {
        chip8_tick();
        clock_prev = clock_now;
    }
}

void chip8_keydown(unsigned char k, int x, int y) {
  if(k == 27)
    exit(0);

  if(k == '1')      key[0x1] = 1;
  else if(k == '2') key[0x2] = 1;
  else if(k == '3') key[0x3] = 1;
  else if(k == '4') key[0xC] = 1;

  else if(k == 'q') key[0x4] = 1;
  else if(k == 'w') key[0x5] = 1;
  else if(k == 'e') key[0x6] = 1;
  else if(k == 'r') key[0xD] = 1;

  else if(k == 'a') key[0x7] = 1;
  else if(k == 's') key[0x8] = 1;
  else if(k == 'd') key[0x9] = 1;
  else if(k == 'f') key[0xE] = 1;

  else if(k == 'z') key[0xA] = 1;
  else if(k == 'x') key[0x0] = 1;
  else if(k == 'c') key[0xB] = 1;
  else if(k == 'v') key[0xF] = 1;
}

void chip8_keyup(unsigned char k, int x, int y) {
  if(k == '1')      key[0x1] = 0;
  else if(k == '2') key[0x2] = 0;
  else if(k == '3') key[0x3] = 0;
  else if(k == '4') key[0xC] = 0;

  else if(k == 'q') key[0x4] = 0;
  else if(k == 'w') key[0x5] = 0;
  else if(k == 'e') key[0x6] = 0;
  else if(k == 'r') key[0xD] = 0;

  else if(k == 'a') key[0x7] = 0;
  else if(k == 's') key[0x8] = 0;
  else if(k == 'd') key[0x9] = 0;
  else if(k == 'f') key[0xE] = 0;

  else if(k == 'z') key[0xA] = 0;
  else if(k == 'x') key[0x0] = 0;
  else if(k == 'c') key[0xB] = 0;
  else if(k == 'v') key[0xF] = 0;
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: ./chip-8 <game>\n");
    exit(2);
  }

  // Initialize the Chip8 system and load the game into the memory
  chip8_init();
  chip8_loadgame(argv[1]);

  // Setup OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowSize(GFX_ROWS, GFX_COLS);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Chip-8 by Ilias Karimalis");

  glutDisplayFunc(draw);
  glutIdleFunc(loop);
  glutReshapeFunc(reshape_window);

  glutKeyboardFunc(chip8_keydown);
  glutKeyboardUpFunc(chip8_keyup);

  gfx_setup();

  gettimeofday(&clock_prev, NULL);

  glutMainLoop();

  return 0;
}
