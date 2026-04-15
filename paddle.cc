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
      : Circle(x_, y_, r_), width(width_), paddle(x_, y_, r_),
        last_delta(l_dx_, l_dy_) {}

Paddle::~Paddle() {}

Circle Paddle::getCircle() const {
    return Circle(paddle);
}

void Paddle::setCentrePaddle(double x_, double y_){
    paddle.setCentre(x_, y_);
}

double Paddle::getLast_delta() const { // Dernier vecteur de déplacement
    double x = last_delta.getCoordinate().first;
    return x;
}

std::pair<double,double> Paddle::getCenter_paddle() const { // Centre du Paddle
    return paddle.getCentre();
}

double Paddle::getWidth() const {
    return width;
}

void Paddle::draw_paddle() const {
    draw_arc(getCenter_paddle().first, getCenter_paddle().second, paddle.getRadius());
}