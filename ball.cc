// ball.h  : Définition des méthodes, constructeurs et destructeurs de la classe de
//           type Ball. Cet object sera visible dans l'arène et intéragira avec le 
//           Paddle et les Brick.
//
//
// Version 1.0
//

#include <string>
#include <iostream>
#include <sstream>
#include "ball.h"
#include "constants.h"
#include "tools.h"
#include "graphic.h"


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

// double Ball::getDelta() const { 
//     double x = delta.getCoordinate().first;
//     return x; 
// }

std::pair<double,double> Ball::getCentre_ball() const {
    return ball.getCentre();
}

std::pair<double,double> Ball::getDeltaVector() const {
    return delta.getCoordinate(); // vecteur vitesse de la balle dans l'arène
}


bool Ball::is_in_arena() const { // S'assure que la balle est dans 
    double x = getCentre_ball().first;
    double y = getCentre_ball().second;
    return (x >= 0) && (x <= arena_size) && (y >= 0) && (y <= arena_size);
}

void Ball::draw_ball() const {
    draw_circles(getCentre_ball().first, getCentre_ball().second, radius);
}