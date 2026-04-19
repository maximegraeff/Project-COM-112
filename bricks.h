// objects.h  : Déclaration des méthodes, constructeurs et destructeurs des classes de
//              type Brick (RwBrick: Rainbow Brick, BallBrick: Ball Brick, SpltBrick:
//              Split Brick), Paddle et Ball. Ces objects seront visibles dans l'arène.
//
//
// Version 3.0
//


#ifndef OBJECTS_H
#define OBJECTS_H

#include <string> 
#include <sstream>
#include "tools.h"
#include "ball.h"

using namespace std;


//-------------------------- Déclaration de la classe Brick ---------------------------
// Super-classe Brick constitutée d'une rectangle. Les autres Brick sont dérivées de 
// cette classe.

class Brick {

    public:
    
        Rectangle getRectangle() const;
        virtual int getType() const;
        virtual int getHitPoints() const;
        virtual void draw_brick() const = 0;

        Brick(double x_, double y_, double length_, double width_, int hp_ = 0,  
              bool is_destroyed_ = false);
        virtual ~Brick();

    protected:

        Rectangle brick; 
        int hp; // hit_point de la brick en question
        bool is_destroyed; 
};


//------------------------- Déclaration de la classe RwBrick --------------------------
// Sous-classe RwBrick(Rainbow Brick) héritée de Brick. Spécificité : Change de couleur
// à chaque coup dans un ordre prédéfini dans la liste colors.

class RwBrick : public Brick {

    public:

        int getType() const override;
        void draw_brick() const override;
        int getHitPoints() const override;

        RwBrick(double x_, double y_, double length_, double width_, int hp_,
                bool is_destroyed_ = false, 
                int current_color_i_ = 0);
        ~RwBrick();

    private:

        int current_color_i;
};


//------------------------ Déclaration de la classe BallBrick -------------------------
// Sous-classe BallBrick(Ball Brick) héritée de Brick. Spécificité : Possède une balle 
// en son centre qui est libérée à la mort de la BallBrick.

class BallBrick : public Brick {
    
    public:

        int getType() const override;
        void draw_brick() const override;
        int getHitPoints() const override;

        BallBrick(double x_, double y_, double length_, double width_,  
                  bool is_destroyed_ = false, double b_radius_ = 1,
                  double dx_ = 0, double dy_ = 0, bool is_b_destroyed_ = false);
        ~BallBrick();

    private:
            
        Ball ball_inside;
};


//------------------------ Déclaration de la classe SpltBrick -------------------------
// Sous-classe SpltBrick(Split Brick) héritée de Brick. Spécificité : Se divise en 4 à 
// chaque coup et change de couleurs en conséquence.

class SpltBrick : public Brick {

    public:

    int getType() const override;
    void draw_brick() const override;
    int getHitPoints() const override ;

        SpltBrick(double x_, double y_, double length_, double width_,
                  bool is_destroyed_ = false, int split_count_ = 0,
                  int children_created_ = 0, int current_color_i_ = 0);
        ~SpltBrick();

    private:

        int split_count;
        int children_created; // Nombre de SpltBrick crées
        int current_color_i;
};


#endif
