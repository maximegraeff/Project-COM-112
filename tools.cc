// tools.cc  : Définiton des classes de formes géométriques (Point, Rectangle, Circle)
//             et des fonctions intersect
//
// Version 2.5
//


#include <string> 
#include <sstream>
#include <iostream>
#include <cmath>
#include "tools.h"
#include "constants.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


//--------------------------- Définition de la classe Point ---------------------------

std::pair<double,double> Point::getCoordinate() const { // Coordonées du point
    return {x, y};
}

Point::Point(double x_, double y_):x(x_), y(y_) {}

Point::~Point() {}


//------------------------- Définition de la classe Rectangle -------------------------

std::pair<double,double> Rectangle::getCentre() const {
    return centre.getCoordinate();
}

double Rectangle::getLength() const {
    return length;
}

double Rectangle::getWidth() const {
    return width;
}

double Rectangle::surface() const {
    return length*width;
}

Rectangle::Rectangle(double x_, double y_, double length_, double width_)
        :centre(x_, y_), length(length_ >= 0 ? length_ : 0), 
         width(width_ >= 0 ? width_ : 0) {}

Rectangle::~Rectangle() {}


//-------------------------- Définition de la classe Circle ---------------------------

std::pair<double,double> Circle::getCentre() const {
    return centre.getCoordinate();
}

double Circle::getRadius() const {
    return radius;
}

double Circle::surface() const {
    return M_PI * pow(radius, 2);
}

void Circle::setCentre(double x_, double y_) {
    centre = Point(x_, y_);
}

Circle::Circle (double x_, double y_, double radius_)
      :centre(x_, y_), radius(radius_ >= 0 ? radius_ : 0) {}


Circle::~Circle() {}


//------------------------ Définition des fonctions intersect -------------------------

// Intersection entre un cercle et un rectangle
bool intersects(Circle c, Rectangle r, double e) {

    double x_c = c.getCentre().first;
    double y_c = c.getCentre().second;
    double r_c = c.getRadius();
    double x_r = r.getCentre().first;
    double y_r = r.getCentre().second;
    double l = r.getLength();
    double w = r.getWidth();

    double closest_x = max(x_r - w/2, min(x_c, x_r + w/2));
    double closest_y = max(y_r - l/2, min(y_c, y_r + l/2));
    // Calcul du point du rectangle le plus proche du centre du cercle

    double dx = x_c - closest_x;
    double dy = y_c - closest_y;
    // Calcul de la distance entre le point precedent et le centre du cercle

    if (sqrt(pow(dx, 2) + pow(dy, 2)) - r_c < e){
        return true;
    } else return false;
}

// Intersection entre deux cercles
bool intersects(Circle c1, Circle c2, double e) {

    double x1 = c1.getCentre().first;
    double y1 = c1.getCentre().second;
    double r1 = c1.getRadius();
    double x2 = c2.getCentre().first;
    double y2 = c2.getCentre().second;
    double r2 = c2.getRadius();

    double dist_ctr = sqrt(pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2));

    if (dist_ctr - r1 - r2 < e){ // Intersection si la distance entre les centres est 
        return true;         // plus petite que la somme de leur rayons. 
    } else return false;
}

// Intersection entre deux rectangles
bool intersects(Rectangle r1, Rectangle r2, double e) {

    double x1 = r1.getCentre().first;
    double y1 = r1.getCentre().second;
    double l1 = r1.getLength();
    double w1 = r1.getWidth();
    double x2 = r2.getCentre().first;
    double y2 = r2.getCentre().second;
    double l2 = r2.getLength();
    double w2 = r2.getWidth();

    double dx = abs(x1 - x2);
    double dy = abs(y1 - y2);
    double cx = (w1 + w2)/2;
    double cy = (l1 + l2)/2;

    if ((dx - cx < e) and (dy - cy < e)){ //Intersection si les distances entre les 
                                          //deux centres 
        return true;             //sont inférieures aux moyennes de leurs côtés selon 
    } else return false;      //x et y.
}

// Controle si l'objet est dans l'arène
bool is_inside_arena(double x, double y, double w, double e) {
    if (((x - w/2) >= e) and ((x + w/2) <= arena_size - e) 
        and ((y - w/2) >= e) and ((y + w/2) <= arena_size - e)) {
        return true;
    } else return false;
}

