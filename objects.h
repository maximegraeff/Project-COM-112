// objects.h  : 3 déclarations de classes les objets briques, balle et raquette
//
//
// Version 1.0 du 07.03.2025
//
// Raf : déclaration des classes brick, ball, padel

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string> 
#include <sstream>
#include "tools.h"

class Brick {
    public:
        Brick(double x_, double y_, double width_, char color_);
        ~Brick(){}

    private:
        Square brick;
        char color;

};


#endif