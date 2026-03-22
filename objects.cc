// objects.cc  : # fonctions qui spécifient la structures de niveau intermédiaire tel 
//                 que les briques, les balles, etc...            
//
// Version 1.0 du 05.03.2025
//
// Raf: gestion des erreurs

#include <string>
#include <iostream>
#include <sstream>
#include "objects.h"
#include "constants.h"

int Brick::brick_count(0);
int Ball::ball_count(0);

//--------------------------- Définition de la classe Brick ---------------------------

Brick::Brick(double x_, double y_, double length_, double width_, int hp_ = 0,  // Pq la couleur ? C'est lier aux hp de la brick non ?
             char color_ = 'rouge', bool is_destroyed_ = false)
      : brick(0,0,0,0), hp(hp_), color(color_), is_destroyed(is_destroyed_)  {

        brick_count++;
}

Brick::~Brick(){
    brick_count--;
}

Rectangle Brick::getRectangle() const {
    return Rectangle(brick);
}

void Brick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}

bool Brick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}

void Brick::destroy(){
    std::cout << "Work in progress" << std::endl;
}

//-------------------------- Définition de la classe RwBrick --------------------------

RwBrick::RwBrick(double x_, double y_, double length_, double width_, char color_, 
                 int hp_, bool is_destroyed_, int current_color_i_)
        : Brick(0,0,0,0, color_, hp_, is_destroyed_), 
                current_color_i(current_color_i_){
             
        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_);
        } 
        current_color_i = hp_;
        brick_count++;
}

RwBrick::~RwBrick(){
    brick_count--;
}

char RwBrick::getColor(){
    return colors[current_color_i];
}

void RwBrick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}

bool RwBrick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}

void RwBrick::destroy(){
    std::cout << "Work in progress" << std::endl;
}


//------------------------- Définition de la classe SpltBrick -------------------------

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_, char color_, 
                int hp_, bool is_destroyed_, int split_count_, int children_created_, 
                int current_color_i_)
          : Brick(0,0,0,0, color_, hp_, is_destroyed_), split_count(split_count_), 
            children_created(children_created_), current_color_i(current_color_i_){

        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_); 
        }

        current_color_i = hp_;
        brick_count++;
}

SpltBrick::~SpltBrick(){
    brick_count--;
}

void SpltBrick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}
bool SpltBrick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}
void SpltBrick::destroy(){
    std::cout << "Work in progress" << std::endl;
}
void SpltBrick::split(){
    std::cout << "Work in progress" << std::endl;
}


//-------------------------- Définition de la classe Paddle ---------------------------

Paddle::Paddle(double x_, double y_, double r_, char color_ = 'noir', double l_dx_, 
               double l_dy_)
       : paddle(x_, y_, r_), color(color_), last_delta(l_dx_, l_dy_){
    if (y_ <=0){
        paddle = Circle(x_, y_, r_);
    }

}

Circle Paddle::getCircle() const {
    return Circle(paddle);
}

double Paddle::getLast_delta() const {
    return last_delta.getCoordinate();
}

double Paddle::getCenter_paddle(){
    return paddle.getCentre();
}

void Paddle::move_to(double target_x){
    std::cout << "Work in progress" << std::endl;
}


//--------------------------- Définition de la classe Ball ----------------------------

Ball::Ball(double x_, double y_, double radius_, double dx_, double dy_, char color_ = 'noir', 
           bool is_destroyed_ = false)
     : ball(0,0,0), delta(dx_, dy_), color(color_), 
       is_destroyed(is_destroyed_){

    ball_count++;

}

Ball::~Ball(){
    ball_count--;
}

Circle Ball::getCircle() const {
    return Circle(ball);
}

double Ball::getDelta() const {
    return delta.getCoordinate();
}

double Ball::getCentre_ball() {
    return ball.getCentre();
}

void Ball::rebond_arena(){
    std::cout << "Work in progress" << std::endl;
}

void Ball::rebond_brick(const Brick& brick){
    std::cout << "Work in progress" << std::endl;
}

void Ball::rebond_paddle(const Paddle& paddle){
    std::cout << "Work in progress" << std::endl;
}

bool Ball::is_in_arena(){
    double x,y;
    x,y = getCentre_ball();
    if ((x >= arena_size) or (y >= arena_size)){
        return true;
    }
    else {return false;}
}