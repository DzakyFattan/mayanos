#ifndef _SYS_H
#define _SYS_H

// screen size constant
#define SCREEN_HEIGHT 0x1900
#define SCREEN_WIDTH 0x50

// variabel untuk tracking posisi cursor
extern int cursor_x = 0x0;
extern int cursor_y = 0x0;

// bios color
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

#endif