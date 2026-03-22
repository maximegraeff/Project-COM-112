// objects.h  : 3 déclarations de classes les objets briques, balle et raquette
//
//
// Version 1.0 du 07.03.2025
//
// Raf : déclaration des classes brick, ball, padel

/*
Brick
│
├── NormalBrick
├── RainbowBrick
└── SplitBrick
*/

#ifndef OBJECTS_H
#define OBJECTS_H

#include <string> 
#include <sstream>
#include "tools.h"

class Brick {
    public:
    
        Rectangle getRectangle() const;
        void on_hit();
        bool is_alive();
        void destroy();
        

        Brick(double x_, double y_, double length_, double width_, int hp_,  
               char color_, bool is_destroyed_);
        ~Brick(){}

    protected:
        static int brick_count;
        Rectangle brick; //Position, width, length
        char color; //Brick color
        int hp; //lives points
        bool is_destroyed; 


};


class RwBrick : public Brick {

    public:

        
        void on_hit();
        bool is_alive();
        void destroy();
        

        char getColor(){}

        RwBrick(double x_, double y_, double length_, double width_, int hp_, 
                char color_, bool is_destroyed_, int current_color_i_);
        ~RwBrick(){}

    private:
        const char colors[7] = {'rouge', 'orange', 'jaune', 'vert', 'cyan', 'bleu', 
                                'violet'};
        int current_color_i;
};


class SpltBrick : public Brick {

    public:

        
        void on_hit();
        bool is_alive();
        void destroy();
        void split();
        

        int getSplit_count(){}
        int getChildren_creatd(){}
        char getColor(){}

        SpltBrick(double x_, double y_, double length_, double width_, int hp_, 
                  char color_, bool is_destroyed_, int split_count, int children_recated, 
                  int current_color_i_);
        ~SpltBrick(){}

    private:
        int split_count; //Nombre de fois que la brique est cassée
        int children_created; //Nombre de SpltBrick crées
        const char colors[7] = {'rouge', 'orange', 'jaune', 'vert', 'cyan', 'bleu', 
                                'violet'};
        int current_color_i;


};


class Paddle {

    public:
       
        Circle getCircle() const;
        double getLast_delta() const;
        double getCenter_paddle();
        void move_to(double target_x);

        Paddle(double x_, double y_, double r_, char color_, double l_dx_, double l_dy_);
        ~Paddle(){};
        
    private:
        char color;
        Circle paddle;  // seul l'arc supérieur est affiché
        Point last_delta;
};


class Ball {

    public:
    
        Circle getCircle() const;
        double getDelta() const;
        double getCentre_ball();

        void rebond_arena();
        void rebond_brick(const Brick& brick);
        void rebond_paddle(const Paddle& paddle);
        bool is_in_arena();

        Ball(double x_, double y_, double radius_, double dx_, double dy_, char color_, 
             bool is_destroyed_);
        ~Ball(){};
       
    private:
        static int ball_count;
        char color;
        double radius;
        bool is_destroyed;
        Circle ball;   // composition avec tools
        Point delta;    // vecteur déplacement
};


#endif