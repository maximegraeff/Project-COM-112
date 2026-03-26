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
#include "objects.h"
#include "constants.h"



//-------------------------- Définition de la classe Paddle ---------------------------
// Paddle consiruté d'un cercle dont le centre est hors de l'arène

Paddle::Paddle(double x_, double y_, double r_, char color_, double l_dx_, 
               double l_dy_)
      : color(color_), paddle(x_, y_, r_), last_delta(l_dx_, l_dy_) {
    if (y_ <=0){
        paddle = Circle(x_, y_, r_);
    }
}
Paddle::~Paddle() {}

Circle Paddle::getCircle() const {
    return Circle(paddle);
}

double Paddle::getLast_delta() const { // Dernier vecteur de déplacement
    double x = last_delta.getCoordinate().first;
    return x;
}

std::pair<double,double> Paddle::getCenter_paddle() const { // Centre du Paddle
    return paddle.getCentre();
}


//--------------------------- Définition de la classe Ball ----------------------------
// Ball constituée d'un cercle et d'un vecteur de classe Point

Ball::Ball(double x_, double y_, double radius_, double dx_, double dy_, char color_, 
           bool is_destroyed_)
    : color(color_), radius(radius_), is_destroyed(is_destroyed_),
      ball(x_, y_, radius_), delta(dx_, dy_) {}

Ball::~Ball(){}

Circle Ball::getCircle() const {
    return Circle(ball);
}

double Ball::getDelta() const { // vecteur vitesse de la balle dans l'arène
    double x = delta.getCoordinate().first;
    return x; 
}

std::pair<double,double> Ball::getCentre_ball() const {
    return ball.getCentre();
}


bool Ball::is_in_arena() const { // S'assure que la balle est dans 
    double x = getCentre_ball().first;
    double y = getCentre_ball().second;
    return (x >= 0) && (x <= arena_size) && (y >= 0) && (y <= arena_size);
}


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



//------------------------- Définition de la classe SpltBrick -------------------------
// Sous-classe SpltBrick(Split Brick) héritée de Brick. Spécificité : Se divise en 4 à 
// chaque coup et change de couleurs en conséquence.

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_,
                char color_, bool is_destroyed_, int split_count_, int children_created_, 
                int current_color_i_)
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
