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
#include <math.h>
#include "bricks.h"
#include "constants.h"
#include "tools.h"
#include "graphic.h"

//--------------------------- Définition de la classe Brick ---------------------------
// Super-classe Brick constitutée d'une rectangle. Les autres Brick sont dérivées de 
// cette classe.

Brick::Brick(double x_, double y_, double length_, double width_, int hp_)
    : brick(x_, y_, length_, width_), hp(hp_) {count++;}

Brick::~Brick() {count--;}

int Brick::count(0);

Rectangle Brick::getRectangle() const {
    return Rectangle(brick);
}

int Brick::get_brick_count() {
    return count;
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

RwBrick::RwBrick(double x_, double y_, double length_, double width_, int hp_)
        : Brick(x_, y_ , length_, width_, hp_) {
            hp = hp_;
        }

RwBrick::~RwBrick(){}

int RwBrick::getType() const {
    return 0;
}

void RwBrick::draw_brick() const { // Fonction de dessin dans l'interface
    draw_rectangles(brick.getCentre().first, brick.getCentre().second,
                    brick.getLength(), brick.getWidth(), hp);
}

int RwBrick::getHitPoints() const {
    return hp;
}

int RwBrick::get_destroyed() {
    if (hp <= 1) return 0;
    else {
        hp--;
        return 1;
    }
}

//------------------------- Définition de la classe BallBrick -------------------------
// Sous-classe BallBrick(Ball Brick) héritée de Brick. Spécificité : Possède une balle 
// en son centre qui est libérée à la mort de la BallBrick.

BallBrick::BallBrick(double x_, double y_, double length_, double width_)
         : Brick(x_, y_, length_, width_) {}

BallBrick::~BallBrick() {}

int BallBrick::getType() const {
    return 1;
}

void BallBrick::draw_brick() const { // Fonction de dessin dans l'interface
    draw_rectangles(brick.getCentre().first, brick.getCentre().second, 
                    brick.getLength(), brick.getWidth());
    draw_circles(brick.getCentre().first, brick.getCentre().second, new_ball_radius);
}

int BallBrick::getHitPoints() const {
    return hp;
}

int BallBrick::get_destroyed() {
    return 2;
}



//------------------------- Définition de la classe SpltBrick -------------------------
// Sous-classe SpltBrick(Split Brick) héritée de Brick. Spécificité : Se divise en 4 à 
// chaque coup et change de couleurs en conséquence.

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_)
          : Brick(x_, y_, length_, width_){

        hp = floor(log2((length_ + split_brick_gap)/
                                (brick_size_min + split_brick_gap)) + 1);
}

SpltBrick::~SpltBrick(){}

int SpltBrick::getType() const {
    return 2;
}

void SpltBrick::draw_brick() const { // Fonction de dessin dans l'interface
    draw_rectangles(brick.getCentre().first, brick.getCentre().second, 
                    brick.getLength(), brick.getWidth(), hp); 

    int s = hp - 1;
    while (s > 0) {
        int d = pow(2,s-1) - 1;
        for (int j = pow(2, s-1) ; j > 0; --j) {
            draw_rectangles(brick.getCentre().first, brick.getCentre().second - 
                            (((split_brick_gap + brick.getLength())/pow(2, s)) * d),
                            split_brick_gap, brick.getWidth(), s);
            draw_rectangles(brick.getCentre().first - 
                            (((split_brick_gap + brick.getLength())/pow(2, s)) * d),
                            brick.getCentre().second, brick.getLength(), 
                            split_brick_gap, s);
            d = d - 2;
        }
    s--;
    }
    
}

int SpltBrick::getHitPoints() const {
    return hp;
}

int SpltBrick::get_destroyed() {
    if (hp <= 1) return 0;
    else {
        hp--;
        return 3;
    }
}