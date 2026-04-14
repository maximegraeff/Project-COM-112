#include <iostream>
#include "graphic.h"
#include "graphic_gui.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}

void draw_rectangles(double center_x, double center_y, double length, double width,
                     int hp)
{
    if (!ptcr or !*ptcr) return;  // protection si ptcr est nullptr
    get_brick_color(hp);
    (*ptcr)->rectangle(center_x - width / 2, center_y - length / 2, width, length);
    (*ptcr)->fill();
}

void draw_balls(double center_x, double center_y, double radius)
{
    if (!ptcr or !*ptcr) return;  // protection si ptcr est nullptr
    set_color(BLACK);
    (*ptcr)->arc(center_x, center_y, radius, 0, 2 * M_PI);
    (*ptcr)->fill();
}

void get_brick_color(int hp)
{
    switch (hp)
    {
    case 1:
        set_color(RED);
        cout << "color : red" << endl;
        break;
    case 2:
        set_color(ORANGE);
        cout << "color : orange" << endl;
        break;
    case 3:
        set_color(YELLOW);
        cout << "color : yellow" << endl;   
        break;
    case 4:
        set_color(GREEN);
        cout << "color : green" << endl;
        break;
    case 5:
        set_color(CYAN);
        cout << "color : cyan" << endl;
        break;
    case 6:
        set_color(BLUE);
        cout << "color : blue" << endl;
        break;
    case 7:
        set_color(PURPLE);
        cout << "color : purple" << endl;
        break;
    default:
        set_color(GREY);
        break;
    }
}

// local functions
void set_color(unsigned color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}