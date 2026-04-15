#ifndef GRAPHIC_H
#define GRAPHIC_H

void draw_rectangles(double x_, double y_, double length, double width, int hp = 1);
void draw_circles(double x_, double y_, double r, int c = 8);
void draw_arc(double x_, double y_, double r, int c = 8);
void draw_arena();
void get_color(int color);
void set_color(unsigned color);

enum Color
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    BLACK,
    GREY,
    WHITE,
};
#endif
