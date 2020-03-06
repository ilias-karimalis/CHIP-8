#include "chip8.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <sys/time.h>

#define PIXEL_SIZE 5

#define CLOCK_HZ 60
#define CLOCK_RATE_MS ((int) ((1.0 / CLOCK_HZ) * 1000 + 0.5))

#define BLACK 0
#define WHITE 255

#define SCREEN_ROWS (GFX_ROWS * PIXEL_SIZE)
#define SCREEN_COLS (GFX_COLS * PIXEL_SIZE)

unsigned char screen[SCREEN_ROWS][SCREEN_COLS][3];

struct timeval clock_prev;

int timediff_ms(struct timeval *end, struct timeval *start);
void display();
void loop();
void reshape_window();
void keypress(unsigned char k, int x, int y);
void keyrelease(unsigned char k, int x, int y);
void paint_cell(int row, int col, unsigned char color);
int keymap(unsigned char k);
void gfx_setup();

void paint_pixel(int row, int col, unsigned char color) {
    row = SCREEN_ROWS - 1 - row;
    screen[row][col][0] = screen[row][col][1] = screen[row][col][2] = color;
}

int timediff_ms(struct timeval *end, struct timeval *start) {
    int diff =  (end->tv_sec - start->tv_sec) * 1000 +
                (end->tv_usec - start->tv_usec) / 1000;
    return diff;
}

void gfx_setup() {
    memset(screen, BLACK, sizeof(unsigned char) * SCREEN_ROWS * SCREEN_COLS * 3);
    glClear(GL_COLOR_BUFFER_BIT);
}

int keymap(unsigned char k) {
    switch (k) {
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case '4': return 0xc;

        case 'q': return 0x4;
        case 'w': return 0x5;
        case 'e': return 0x6;
        case 'r': return 0xd;

        case 'a': return 0x7;
        case 's': return 0x8;
        case 'd': return 0x9;
        case 'f': return 0xe;
                  
        case 'z': return 0xa;
        case 'x': return 0x0;
        case 'c': return 0xb;
        case 'v': return 0xf;

        default:  return -1;
    }
}

void keypress(unsigned char k, int x, int y) {
    (void) x; (void) y;

    int index = keymap(k);
    if (index >= 0) {
        key[index] = 1;
    }
}

void keyrelease(unsigned char k, int x, int y) {
    (void) x; (void) y;

    int index = keymap(k);
    if (index >= 0) {
        key[index] = 0;
    }
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

void display() {
    int row, col;

    glClear(GL_COLOR_BUFFER_BIT);

    for (row = 0; row < GFX_ROWS; row++) {
        for (col = 0; col < GFX_COLS; col++) {
            paint_cell(row, col, gfx[col + 64*row] ? WHITE : BLACK);
        }
    }
}

void loop() {
    struct timeval clock_now;
    gettimeofday(&clock_now, NULL);

    chip8_emulatecycle();

    if (draw_flag) {
        display();
        draw_flag = false;
    }

    if (timediff_ms(&clock_now, &clock_prev) >= CLOCK_RATE_MS) {
        chip8_tick();
        clock_prev = clock_now;
    }
}

void reshape_window(GLsizei w, GLsizei h) {
    (void) w; (void) h;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: ./play <game>\n");
        exit(2);
    }

    chip8_init();
    chip8_loadgame(argv[1]);

    // Setup OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(SCREEN_COLS, SCREEN_ROWS);
    glutInitWindowSize(0, 0);
    glutCreateWindow("Chip-8 by Ilias Karimalis");

    glutDisplayFunc(display);
    glutIdleFunc(loop);
    glutReshapeFunc(reshape_window);

    glutKeyboardFunc(keypress);
    glutKeyboardUpFunc(keyrelease);

    gfx_setup();

    gettimeofday(&clock_prev, NULL);

    glutMainLoop();

    return 0;
}