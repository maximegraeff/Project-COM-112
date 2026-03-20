// objects.cc  : # fonctions qui spécifient la structures de niveau intermédiaire tel 
//                 que les briques, les balles, etc...            
//
// Version 1.0 du 05.03.2025
//
// Raf: création brick avec le principe de hierarchie

#include <string> 
#include <sstream>
#include "objects.h"
#include "constants.h"

int Brick::brick_count(0);

//--------------------------- Définition de la classe Brick ---------------------------

Brick::Brick(double x_, double y_, double length_, double width_, char color_ = 'rouge',
             int hp_ = 0, bool is_destroyed_ = false)
    : brick(0,0,0,0), color(color_), hp(hp_), is_destroyed(is_destroyed_)  {

        brick_count++;
}

Brick::~Brick(){
    brick_count--;
}

//-------------------------- Définition de la classe RwBrick --------------------------

RwBrick::RwBrick(double x_, double y_, double length_, double width_, char color_, 
                 int hp_, bool is_destroyed_, int current_color_i_)
        : Brick(0,0,0,0, color_, hp_, is_destroyed_), 
                current_color_i(current_color_i_){
             
        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_);
        } 
        current_color_i = hp_;
        brick_count++;
}

RwBrick::~RwBrick(){
    brick_count--;
}

char RwBrick::getColor(){
    return colors[current_color_i];
}

//------------------------- Définition de la classe SpltBrick -------------------------

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_, char color_, 
                int hp_, bool is_destroyed_, int split_count_, int children_created_, 
                int current_color_i_)
            : Brick(0,0,0,0, color_, hp_, is_destroyed_), 
            split_count(split_count_), children_created(children_created_), 
            current_color_i(current_color_i_){

        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_); 
        }

        current_color_i = hp_;
        brick_count++;

            }




