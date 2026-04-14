// objects.cc  : Définition des méthodes, constructeurs et destructeurs des classes
//               de type Brick (RwBrick: Rainbow Brick, BallBrick: Ball Brick,
//               SpltBrick: Split Brick), Paddle et Ball. Ces objects seront visibles
//               dans l'arène.         
//
// Version 3.0
//

#include <string>
#include <iostream>
#include <sstream>
#include "bricks.h"
#include "constants.h"


//--------------------------- Définition de la classe Brick ---------------------------
// Super-classe Brick constitutée d'une rectangle. Les autres Brick sont dérivées de 
// cette classe.

Brick::Brick(double x_, double y_, double length_, double width_,
             int hp_, char color_, bool is_destroyed_)
    : brick(x_, y_, length_, width_), color(color_), hp(hp_),
      is_destroyed(is_destroyed_) {}

Brick::~Brick(){}

Rectangle Brick::getRectangle() const {
    return Rectangle(brick);
}

int Brick::getType() const {
    return -1;
}

int Brick::getHitPoints() const {
    return hp;
}


//-------------------------- Définition de la classe RwBrick --------------------------
// Sous-classe RwBrick(Rainbow Brick) héritée de Brick. Spécificité : Change de couleur
// à chaque coup dans un ordre prédéfini dans la liste colors.

RwBrick::RwBrick(double x_, double y_, double length_, double width_, int hp_,
                 char color_, bool is_destroyed_, int current_color_i_)
        : Brick(0,0,0,0, color_, hp_, is_destroyed_), 
        current_color_i(current_color_i_){
             
    if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
        brick = Rectangle(x_, y_, length_, width_);
    }

    current_color_i = hp_;
}

RwBrick::~RwBrick(){}

char RwBrick::getColor(){ // Changement de la couleur à chaque coup
    return colors[current_color_i];
}

int RwBrick::getType() const {
    return 0;
}


//------------------------- Définition de la classe BallBrick -------------------------
// Sous-classe BallBrick(Ball Brick) héritée de Brick. Spécificité : Possède une balle 
// en son centre qui est libérée à la mort de la BallBrick.

BallBrick::BallBrick(double x_, double y_, double length_, double width_, 
                  char color_, bool is_destroyed_, 
                  double b_radius_, double dx_, double dy_, char b_color_, 
                  bool is_b_destroyed_)
         : Brick(0, 0, 0, 0, color_, is_destroyed_), ball_inside(x_, x_, b_radius_, 
                 dx_, dy_, b_color_, is_b_destroyed_){
    if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
        brick = Rectangle(x_, y_, length_, width_); 
    }

}

BallBrick::~BallBrick() {}

char BallBrick::getColor(){
    return color;
}

int BallBrick::getType() const {
    return 1;
}



//------------------------- Définition de la classe SpltBrick -------------------------
// Sous-classe SpltBrick(Split Brick) héritée de Brick. Spécificité : Se divise en 4 à 
// chaque coup et change de couleurs en conséquence.

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_,
                char color_, bool is_destroyed_, int split_count_, 
                int children_created_, int current_color_i_)
          : Brick(0,0,0,0, color_, is_destroyed_), split_count(split_count_), 
            children_created(children_created_), current_color_i(current_color_i_){

        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_); 
        }

        current_color_i = floor(log2(length_/brick_size_min));
}

SpltBrick::~SpltBrick(){}

char SpltBrick::getColor(){ // Changement de couleur à chaque division
    return colors[current_color_i];
}

int SpltBrick::getType() const {
    return 2;
}
