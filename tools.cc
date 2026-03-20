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

using namespace std;

//------------------------- Définition de la classe Rectangle -------------------------

Point Rectangle::getCentre(){
    return centre;
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
                     length(length_), width(width_){
    if ((width_ >= 0) && (length_ >= 0)){
                width = width_;
                length = length_;
            }
}

Rectangle::~Rectangle(){}


//-------------------------- Définition de la classe Circle ---------------------------

double Circle::getCentre(){
    return centre.getCoordinate();
}

double Circle::getRadius() const {
    return radius;
}

double Circle::surface() const {
    return M_PI * pow(radius, 2);
}

Circle::Circle (double x_, double y_, double radius_):centre(x_, y_), radius(radius_){
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

bool intersects(Circle c, Rectangle r){
    std::cout << "Work in progress" << std::endl;
}

bool intersects(Circle c1, Circle c2){
    double x1, y1, r1, x2, y2, r2;

    x1, y1 = c1.getCentre();
    r1 = c1.getRadius();

    x2, y2 = c2.getCentre();
    r2 = c2.getRadius();
}

bool intersects(Rectangle r1, Rectangle r2){
    std::cout << "Work in progress" << std::endl;
}

bool is_inside_arena(Rectangle r){
    std::cout << "Work in progress" << std::endl;
}
