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

Ball::Ball(double x_, double y_, double radius_, double dx_, double dy_)
    : radius(radius_), ball(x_, y_, radius_), delta(dx_, dy_), bounces(0){count++;}

Ball::~Ball(){count--;}

int Ball::count(0);

//--------------------------------- Fonctions get -------------------------------------
// Fonction renvoyant le cercle de la ball
Circle Ball::getCircle() const {
    return Circle(ball);
}

// Fonction renvoyant le cercle de la ball à l'étape suivante
Circle Ball::next_circle() const {
    return Circle(getCentre_ball().first + getDeltaVector().first, 
                  getCentre_ball().second + getDeltaVector().second, radius);
}

// Fonction renvoyant le compte des balls du jeu
int Ball::get_ball_count() {
    return count;
}

// Fonction renvoyant le centre de la ball
std::pair<double,double> Ball::getCentre_ball() const {
    return ball.getCentre();
}

// Fonction renvoyant le vecteur de déplacement de la ball
std::pair<double,double> Ball::getDeltaVector() const {
    return delta.getCoordinate(); // vecteur vitesse de la balle dans l'arène
}

//---------------------------- Fonctions de modifications -----------------------------

// Fonction définissant le vecteur de déplacement de la ball
void Ball::setDeltaVector(double dx_, double dy_) {
    delta = Point(dx_, dy_);
}

// Fonction de mise à jour de la position de la ball
void Ball::update_position() {
    double x = getCentre_ball().first;
    double y = getCentre_ball().second;
    ball.setCentre(x + getDeltaVector().first, y + getDeltaVector().second);
}

//-------------------------- Fonctions liés aux rebonds -------------------------------

// Fonction de vérification du nombre de rebonds de la ball
bool Ball::bounce() {
    if (bounces < nb_bounce_max) {
        return true;
    }
    return false;
}

// Fonction d'incrémentation du nombre de rebonds de la ball
void Ball::add_bounce() {
    bounces = bounces + 1;
}

// Fonction de réinitialisation du nombre de rebonds de la ball
void Ball::reset_bounces() {
    bounces = 0;
}

//-------------------------- Fonction de dessin des balls -----------------------------

// Fonction de dessin de la ball
void Ball::draw_ball() const { // Fonction de dessin dans l'interface
    draw_circles(getCentre_ball().first, getCentre_ball().second, radius);
}