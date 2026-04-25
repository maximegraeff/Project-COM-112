// paddle.cc  : Définition des méthodes, constructeurs et destructeurs de la classe de
//              type Paddle. Cet object sera visible dans l'arène et intéragira avec
//              les Ball et les Brick.
//
//
// Version 1.0
//


#include <string>
#include <iostream>
#include <sstream>
#include "paddle.h"
#include "constants.h"

//-------------------------- Définition de la classe Paddle ---------------------------
// Paddle consiruté d'un cercle dont le centre est hors de l'arène

Paddle::Paddle(double x_, double y_, double r_, double width_, 
               double l_dx_, double l_dy_)
      : width(width_), paddle(x_, y_, r_),
        last_delta(l_dx_), paddle_target_x(x_) {}

Paddle::~Paddle() {}

Circle Paddle::getCircle() const {
    return Circle(paddle);
}

void Paddle::setCentrePaddle(double x_, double y_){
    paddle.setCentre(x_, y_);
}

double Paddle::getLast_delta() const { // Dernier vecteur de déplacement
    return last_delta;
}

std::pair<double,double> Paddle::getCenter_paddle() const { // Centre du Paddle
    return paddle.getCentre();
}

double Paddle::getWidth() const {
    return width;
}

void Paddle::draw_paddle() const { // Fonction de dessin dans l'interface
    draw_arc(getCenter_paddle().first, getCenter_paddle().second, paddle.getRadius());
}

void Paddle::set_target_x(double x) { // Suivi du déplacement de la souris
    paddle_target_x = x;
} 

double Paddle::get_target_x() const {
    return paddle_target_x;
}