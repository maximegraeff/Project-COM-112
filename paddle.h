// paddle.h  : Déclaration des méthodes, constructeurs et destructeurs de la classe de
//             type Paddle. Cet object sera visible dans l'arène et intéragira avec les 
//             Ball et les Brick.
//
//
// Version 1.0
//


#ifndef PADDLE_H
#define PADDLE_H

#include <string> 
#include <sstream>
#include "tools.h"
#include "graphic.h"

using namespace std;

//-------------------------- Déclaration de la classe Paddle --------------------------
// Paddle consiruté d'un cercle dont le centre est hors de l'arène

class Paddle {

    public:
       
        Circle getCircle() const;
        void setCentrePaddle(double x_, double y_);
        double getLast_delta() const;
        pair<double,double> getCenter_paddle() const;
        double getWidth() const;
        void draw_paddle() const;
        void set_target_x(double x);
        double get_target_x() const;

        Paddle(double x_, double y_, double r_, double width_, double l_dx_ = 0.0,
               double l_dy_ = 0.0);
        ~Paddle();
        
    private:
        
        double paddle_target_x;
        double width;
        Circle paddle;  // seul l'arc supérieur est affiché
        double last_delta; // dernier vecteur déplcament
};


#endif