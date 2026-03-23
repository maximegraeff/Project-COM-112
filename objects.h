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

class Paddle {

    public:
       
        Circle getCircle() const;
        double getLast_delta() const;
        std::pair<double,double> getCenter_paddle() const;
        //void move_to(double target_x);

        Paddle(double x_, double y_, double r_, char color_ = 'n', double l_dx_ = 0.0,
               double l_dy_ = 0.0);
        ~Paddle();
        
    private:
        char color;
        Circle paddle;  // seul l'arc supérieur est affiché
        Point last_delta;
};


class Ball {

    public:
    
        Circle getCircle() const;
        double getDelta() const;
        std::pair<double,double> getCentre_ball() const;

        //void rebond_arena();
        //void rebond_brick(const Brick& brick);
        //void rebond_paddle(const Paddle& paddle);
        bool is_in_arena() const;

        Ball(double x_, double y_, double radius_, double dx_, double dy_, char color_ = 'n', 
           bool is_destroyed_ = false);
        ~Ball();
       
    private:
        static int ball_count;
        char color;
        double radius;
        bool is_destroyed;
        Circle ball;   // composition avec tools
        Point delta;    // vecteur déplacement
};

class Brick {
    public:
    
        Rectangle getRectangle() const;
        //void on_hit();
        //bool is_alive();
        //void destroy();

        Brick(double x_, double y_, double length_, double width_, int hp_ = 0,  
             char color_ = 'r', bool is_destroyed_ = false);
        ~Brick();

    protected:
        static int brick_count;
        Rectangle brick; //Position, width, length
        char color; //Brick color
        int hp; //lives points
        bool is_destroyed; 


};


class RwBrick : public Brick {

    public:

        //void on_hit();
        //bool is_alive();
        //void destroy();
        

        char getColor();

        RwBrick(double x_, double y_, double length_, double width_, int hp_,
                char color_ = 'r', bool is_destroyed_ = false, int current_color_i_ = 0);
        ~RwBrick();

    private:
        const char colors[7] = {'r', 'o', 'j', 'v', 'c', 'b', 'V'};
        int current_color_i;
};

class BallBrick : public Brick {
    
    public:
        //void on_hit();
        //bool is_alive();
        //void destroy();

        char getColor();

        BallBrick(double x_, double y_, double length_, double width_,  
                  char color_ = 'r', bool is_destroyed_ = false, double b_radius_ = 1,
                  double dx_ = 0, double dy_ = 0, char b_color_ = 'n', bool is_b_destroyed_ = false);
        ~BallBrick();

    private:
            
        Ball ball_inside;
        
};


class SpltBrick : public Brick {

    public:

        
        //void on_hit();
        //bool is_alive();
        //void destroy();
        //void split();
        

        //int getSplit_count();
        //int getChildren_creatd();
        char getColor();

        SpltBrick(double x_, double y_, double length_, double width_,
                char color_ = 'r', bool is_destroyed_ = false, int split_count_ = 0,
                int children_created_ = 0, int current_color_i_ = 0);
        ~SpltBrick();

    private:
        int split_count; //Nombre de fois que la brique est cassée
        int children_created; //Nombre de SpltBrick crées
        const char colors[7] = {'r', 'o', 'j', 'v', 'c', 'b', 'V'};
        int current_color_i;


};


#endif