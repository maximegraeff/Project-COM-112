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
        /*void hit() const;
        bool is_alive();
        void destroy() const;*/

        Brick(double x_, double y_, double length_, double width_, char color_, 
              int hp_, bool is_destroyed_);
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
        char getColor(){}

        RwBrick(double x_, double y_, double length_, double width_, char color_, 
                int hp_, bool is_destroyed_, int current_color_i_);
        ~RwBrick(){}

    private:
        const char colors[7] = {'rouge', 'orange', 'jaune', 'vert', 'cyan', 'bleu', 
                                'violet'};
        int current_color_i;
};

class SpltBrick : public Brick {

    public:
        /*void hit() const;
        bool is_alive();
        void destroy() const;
        void split();*/

        int getSplit_count(){}
        int getChildren_creatd(){}
        char getColor(){}

        SpltBrick(double x_, double y_, double length_, double width_, char color_, 
                int hp_, bool is_destroyed_, int split_count, int children_recated, 
                int current_color_i_);

    private:
        int split_count; //Nombre de fois que la brique est cassée
        int children_created; //Nombre de SpltBrick crées
        const char colors[7] = {'rouge', 'orange', 'jaune', 'vert', 'cyan', 'bleu', 
                                'violet'};
        int current_color_i;


};

#endif