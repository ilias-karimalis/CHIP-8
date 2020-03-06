#include "chip8.h"

#include <stdio.h>
#include <GL/glut.h>

#define CLOCK_HZ 60
#define CLOCK_RATE_MS ((int) ((1.0/ CLOCK_HZ) * 1000 + 0.5))

enum colors {
  Black = 0,
  White = 255
};

#define PIXEL_SIZE 10
int display_width = GFX_ROWS * PIXEL_SIZE;
int display_height = GFX_COLS * PIXEL_SIZE;


unsigned char screen[GFX_ROWS][GFX_COLS][3];

void drawPixel(int x, int y);
void updateQuads();
void display();
void reshape_window(GLsizei w, GLsizei h);
void chip8_keyup(unsigned char k, int x, int y);
void chip8_keydown(unsigned char k, int x, int y);

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
  glutInitWindowPosition(320, 320);
  glutCreateWindow("Chip-8 by Ilias Karimalis");

  glutDisplayFunc(display);
  glutIdleFunc(display);
  //glutReshapeFunc(reshape_window);
  glutKeyboardFunc(chip8_keydown);
  glutKeyboardUpFunc(chip8_keyup);

  glutMainLoop();

  return 0;
}

void drawPixel(int x, int y) {
  glBegin(GL_QUADS);
  glVertex3f((x * PIXEL_SIZE) + 0.0f      , (y * PIXEL_SIZE) + 0.0f, 0.0f);
  glVertex3f((x * PIXEL_SIZE) + 0.0f      , (y * PIXEL_SIZE) + PIXEL_SIZE, 0.0f);
  glVertex3f((x * PIXEL_SIZE) + PIXEL_SIZE, (y * PIXEL_SIZE) + PIXEL_SIZE, 0.0f);
  glVertex3f((x * PIXEL_SIZE) + PIXEL_SIZE, (y * PIXEL_SIZE) + 0.0f, 0.0f);
  glEnd();
}

void updateQuads() {
  for(int y = 0; y < 32; ++y)
    for(int x = 0; x < 64; ++x)
    {
      if(gfx[(y*64) + x] == 0)
        glColor3f(0.0f,0.0f,0.0f);
      else
        glColor3f(1.0f,1.0f,1.0f);

      drawPixel(x, y);
    }
}

void display() {
  chip8_emulatecycle();

  if (draw_flag) {
    glClear(GL_COLOR_BUFFER_BIT);
    updateQuads();
    glutSwapBuffers();
    draw_flag = false;
  }
}

/*
void reshape_window(GLsizei w, GLsizei h) {
  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, h, 0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);

  // Resize quad
  display_width = w;
  display_height = h;
}
*/

void chip8_keydown(unsigned char k, int x, int y)
{
  if(k == 27)    // esc
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

  //printf("Press key %c\n", key);
}

void chip8_keyup(unsigned char k, int x, int y)
{
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
