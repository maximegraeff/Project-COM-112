#include <iostream>
#include <math.h>
#include "graphic.h"
#include "graphic_gui.h"
#include "constants.h"

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

// Fonction de dessin d'un rectangle avec la librairie Cairo de GTKmm
void draw_rectangles(const double x_, const double y_, const double length,
                     const double width, const int hp)
{
    if (!ptcr or !*ptcr) return;  // protection si ptcr est nullptr
    get_color(hp);
    (*ptcr)->rectangle(x_ - width / 2, y_ - length / 2, width, length);
    (*ptcr)->fill();
}

// Fonction de dessin d'un disque avec la librairie Cairo de GTKmm
void draw_circles(const double x_, const double y_, const double r, const int c)
{
    if (!ptcr or !*ptcr) return;  // protection si ptcr est nullptr
    get_color(c);
    (*ptcr)->rectangle(0, 0, arena_size, arena_size);
    (*ptcr)->clip();
    (*ptcr)->arc(x_, y_, r, 0, 2 * M_PI);
    (*ptcr)->fill();
    (*ptcr)->stroke();
}

// Fonction de dessin d'un cercle avec la librairie Cairo de GTKmm
void draw_arc(const double x_, const double y_, const double r, const int c)
{
    if (!ptcr or !*ptcr) return;  // protection si ptcr est nullptr
    get_color(c);
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->rectangle(0, 0, arena_size, arena_size);
    (*ptcr)->clip();
    (*ptcr)->arc(x_, y_, r - (*ptcr)->get_line_width()/2, 0, M_PI);
    (*ptcr)->stroke();
}


// Fonction de dessin de l'arène avec sa bordure grise
void draw_arena() {
    draw_rectangles(arena_size / 2, arena_size / 2, arena_size+1, arena_size+1, 10);
    draw_rectangles(arena_size / 2, arena_size / 2, arena_size, arena_size, 9);
}

void get_color(const int color)
{
    switch (color)
    {
    case 1:
        set_color(RED);
        break;
    case 2:
        set_color(ORANGE);
        break;
    case 3:
        set_color(YELLOW);
        break;
    case 4:
        set_color(GREEN);
        break;
    case 5:
        set_color(CYAN);
        break;
    case 6:
        set_color(BLUE);
        break;
    case 7:
        set_color(PURPLE);
        break;
    case 8:
        set_color(BLACK);
        break;
    case 9:
        set_color(WHITE);
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