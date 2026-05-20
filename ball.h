// ball.h  : Déclaration des méthodes, constructeurs et destructeurs de la classe de
//           type Ball. Cet object sera visible dans l'arène et intéragira avec le 
//           Paddle et les Brick.
//
//
// Version 1.0
//


#ifndef BALL_H
#define BALL_H

#include <string> 
#include <sstream>
#include "tools.h"

using namespace std;

//-------------------------- Déclaration de la classe Ball ----------------------------
// Ball constituée d'un cercle et d'un vecteur de classe Point

class Ball {

    public:
    
        Circle getCircle() const;
        Circle next_circle() const;
        static int get_ball_count();
        pair<double,double> getCentre_ball() const;
        pair<double,double> getDeltaVector() const;

        void setDeltaVector(double dx_, double dy_);
        void update_position();

        bool bounce();
        void add_bounce();
        void reset_bounces();

        void draw_ball() const;

        Ball(double x_, double y_, double radius_, double dx_, double dy_);
        ~Ball();
       
    private:

        static int count;
        int bounces;
        double radius;
        Circle ball;
        Point delta;    // vecteur déplacement
};

#endif
