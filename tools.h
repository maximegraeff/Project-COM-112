// tools.h  : Déclaration des classes de formes géométriques (Point, Rectangle, Circle)
//            et des fonctions intersect
//
// Version 2.5
//


#ifndef TOOLS_H
#define TOOLS_H

#include <string> 
#include <sstream>
#include <cmath>
#include <utility>

//--------------------------- Déclaration de la classe Point --------------------------

class Point {
    public :
        std::pair<double,double> getCoordinate() const;

        Point (double x_, double y_);
        ~Point(); // Destructeur encore à définir

    private:
        double x;
        double y;

};


//------------------------ Déclaration de la classe Rectangle -------------------------


class Rectangle {
    public :
        std::pair<double,double> getCentre() const;
        double getLength() const;
        double getWidth() const;

        double surface() const;

        Rectangle (double x_, double y_, double length_, double width_);
        ~Rectangle(); // Destructeur encore à définir

    private:
        Point centre;
        double length;
        double width;

};

//-------------------------- Déclaration de la classe Circle --------------------------


class Circle {
    public:
        std::pair<double,double> getCentre() const;
        double getRadius() const;
        double surface() const;
        void setCentre(double x_, double y_);

        Circle (double x_, double y_, double radius_);
        ~Circle(); // Destructeur encore à définir

    private:
        Point centre;
        double radius;
};


//----------------------- Déclaration des fonctions intersect -------------------------

// Intersection entre un cercle et un rectangle
bool intersects(Circle c, Rectangle r);

// Intersection entre deux cercles
bool intersects(Circle c1, Circle c2);

// Intersection entre deux rectangles
bool intersects(Rectangle r1, Rectangle r2);

// Controle si le rectangle est dans l'arène
bool is_inside_arena(Rectangle r); 



#endif