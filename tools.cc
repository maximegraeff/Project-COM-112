// tools.cc  : 3 définitions de classes qui forment les outils utilisés pour créer
//            les objets de briques, balle et de la raquette
//
// Version 1.0 du 07.03.2025
//
// Raf : fonctions intersect

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

std::pair<double,double> Point::getCoordinate() const {
    return {x, y};
}

Point::Point(double x_, double y_):x(x_), y(y_){}

Point::~Point(){}


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

Rectangle::Rectangle(double x_, double y_, double length_, double width_):centre(x_, y_),
                     length(length_ >= 0 ? length_ : 0), width(width_ >= 0 ? width_ : 0){
}

Rectangle::~Rectangle(){}


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

Circle::Circle (double x_, double y_, double radius_):centre(x_, y_), radius(radius_ >= 0 ? radius_ : 0){}


Circle::~Circle(){}


//------------------------ Définition des fonctions intersect -------------------------

bool intersects(Circle c, Rectangle r){

    auto [x_c, y_c] = c.getCentre();
    double r_c = c.getRadius();
    auto [x_r, y_r] = r.getCentre();
    double l = r.getLength();
    double w = r.getWidth();

    double closest_x = max(x_r - w/2, min(x_c, x_r + w/2));
    double closest_y = max(y_r - l/2, min(y_c, y_r + l/2));
    // Calcul du point du rectangle le plus proche du centre du cercle

    double dx = x_c - closest_x;
    double dy = y_c - closest_y;
    // Calcul de la distance entre le point precedent et le centre du cercle

    if (sqrt(pow(dx, 2) + pow(dy, 2)) < sqrt(pow(r_c, 2))){
        return true;
    } else return false;

}

bool intersects(Circle c1, Circle c2){

    auto [x1, y1] = c1.getCentre();
    double r1 = c1.getRadius();
    auto [x2, y2] = c2.getCentre();
    double r2 = c2.getRadius();

    double dist_ctr = sqrt(pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2));

    if (dist_ctr < r1 + r2){ // Intersection si la distance entre les centres est 
        return true;         // plus petite que la somme de leur rayons. 
    } else { return false; }
}

bool intersects(Rectangle r1, Rectangle r2){

    auto [x1, y1] = r1.getCentre();
    double l1 = r1.getLength();
    double w1 = r1.getWidth();
    auto [x2, y2] = r2.getCentre();
    double l2 = r2.getLength();
    double w2 = r2.getWidth();

    double dx = abs(x1 - x2);
    double dy = abs(y1 - y2);
    double cx = (w1 + w2)/2;
    double cy = (l1 + l2)/2;

    if ((dx < cx) && (dy < cy)){ // Intersection si les distances entre les deux centres 
        return true;             // sont inférieures aux moyennes de leurs côtés selon 
    } else { return false; }     // x et y.
}

bool is_inside_arena(Rectangle r){

    auto [x, y] = r.getCentre();
    double l = r.getLength();
    double w = r.getWidth();

    if (((x - w/2) >= 0) && ((x + w/2) <= arena_size) && ((y - l/2) >= 0) 
        && ((y + l/2) <= arena_size)) {
        return true;
    } else {return false;}
}

