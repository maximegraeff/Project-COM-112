// objects.cc  : # fonctions qui spécifient la structures de niveau intermédiaire tel que les briques, les balles, etc...            
//
//
// Version 1.0 du 05.03.2025
//
// Raf: création brick avec le principe de hierarchie

#include <string> 
#include <sstream>
#include "objects.h"
#include "constants.h"

Brick::Brick(double x_, double y_, double width_, char color_)
    : brick(0,0,0), color(color_) {

    if (width_ >= brick_size_min){
        brick = Square(x_, y_, width_);
    }
}


