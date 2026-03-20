// tools.cc  : 3 définitions de classes qui forment les outils utilisés pour créer
//            les objets de briques, balle et de la raquette
//
// Version 1.0 du 07.03.2025
//
// Raf : fonctions intersect

#include <string> 
#include <sstream>
#include <cmath>
#include "tools.h"

using namespace std;

//------------------------- Définition de la classe Rectangle -------------------------

double Rectangle::getCentre() const {
    return x,y;
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

Rectangle::Rectangle(double x_, double y_, double length_, double width_):x(x_), y(y_),
                     length(length_), width(width_){
    if ((width_ >= 0) && (length_ >= 0)){
                width = width_;
                length = length_;
            }
}

Rectangle::~Rectangle(){}


//-------------------------- Définition de la classe Circle ---------------------------

double Circle::getCentre() const {
    return x,y;
}

double Circle::getRadius() const {
    return radius;
}

double Circle::surface() const {
    return M_PI * pow(radius, 2);
}

Circle::Circle (double x_, double y_, double radius_):x(x_), y(y_), radius(radius_){
    if (radius_ >= 0){
        radius = radius_;
    }
}

Circle::~Circle(){}


//--------------------------- Définition de la classe Point ---------------------------

double Point::getCoordinate() const {
    return x,y;
}

Point::Point(double x_, double y_):x(x_), y(y_){}

Point::~Point(){}


//------------------------ Définition des fonctions intersect -------------------------

