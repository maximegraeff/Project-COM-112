#ifndef GRAPHIC_H
#define GRAPHIC_H

void draw_rectangles(double center_x, double center_y, double length, double width,
                     int hp = 1);
void draw_circles(double center_x, double center_y, double radius, int c = 8);
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
