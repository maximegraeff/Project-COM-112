// tools.h  : 3 déclarations de classes qui forment les outils utilisés pour créer
//            les objets de briques, balle et de la raquette
//
// Version 1.0 du 07.03.2025
//
// Raf : fonctions intersect

#ifndef TOOLS_H
#define TOOLS_H

#include <string> 
#include <sstream>
#include <cmath>

class Rectangle {
    public :
        double getCentre() const;
        double getLength() const;
        double getWidth() const;

        double surface() const;

        Rectangle (double x_, double y_, double length_, double width_);
        ~Rectangle(){} // Destructeur encore à définir

    private:
        double x;
        double y;
        double length;
        double width;

};

class Circle {
    public:

        double getCentre() const;
        double getRadius() const;

        double surface() const;

        Circle (double x_, double y_, double radius_);
        ~Circle(){} // Destructeur encore à définir

    private:
        double x;
        double y;
        double radius;
};

class Point {
    public :
        double getCoordinate() const;

        Point (double x_, double y_);
        ~Point(){} // Destructeur encore à définir

    private:
        double x;
        double y;

};


/*bool isInside(double x_, double y_) const{
            double dist = sqrt(pow(x - x_, 2) + pow(y - x_, 2));
            if (dist <= radius) {return true;}
            else {return false;}
        }*/

#endif