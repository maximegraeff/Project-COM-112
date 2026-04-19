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
        pair<double,double> getCentre_ball() const;
        pair<double,double> getDeltaVector() const;
        bool is_in_arena() const;
        void draw_ball() const;
        
        /*void update_position();
        void setDeltaVector(double dx_, double dy_);
        bool bounce();
        void add_bounce();
        void reset_bounces();
        void update_delta();*/

        Ball(double x_, double y_, double radius_, double dx_, double dy_, 
             bool is_destroyed_ = false);
        ~Ball();
       
    private:
        
        int bounces;
        double radius;
        bool is_destroyed;
        Circle ball;
        Point delta;    // vecteur déplacement
        Point delta_temp;
};

#endif
