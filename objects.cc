// objects.cc  : # fonctions qui spécifient la structures de niveau intermédiaire tel 
//                 que les briques, les balles, etc...            
//
// Version 1.0 du 05.03.2025
//
// Raf: gestion des erreurs, read envoie des erreurs

#include <string>
#include <iostream>
#include <sstream>
#include "objects.h"
#include "constants.h"


int Brick::brick_count(0);
int Ball::ball_count(0);        // vrmt nécessaire ? on l'a dans read.cc non ?


//-------------------------- Définition de la classe Paddle ---------------------------

Paddle::Paddle(double x_, double y_, double r_, char color_,
               double l_dx_, double l_dy_)
    : color(color_), paddle(x_, y_, r_), last_delta(l_dx_, l_dy_) {
    if (y_ <=0){
        paddle = Circle(x_, y_, r_);
    }
}
Paddle::~Paddle() {}

Circle Paddle::getCircle() const {
    return Circle(paddle);
}

double Paddle::getLast_delta() const {
    double x = last_delta.getCoordinate().first;
    // double y = last_delta.getCoordinate().second;
    return x; // retourne x; adapter si besoin
}

std::pair<double,double> Paddle::getCenter_paddle() const {
    return paddle.getCentre();
}

/*void Paddle::move_to(double target_x){
    std::cout << "Work in progress" << std::endl;
}*/


//--------------------------- Définition de la classe Ball ----------------------------

Ball::Ball(double x_, double y_, double radius_, double dx_, double dy_,
           char color_, bool is_destroyed_)
    : color(color_), radius(radius_), is_destroyed(is_destroyed_),
      ball(x_, y_, radius_), delta(dx_, dy_) {
    ball_count++;
}

Ball::~Ball(){
    ball_count--;
}

Circle Ball::getCircle() const {
    return Circle(ball);
}

double Ball::getDelta() const {
    double x = delta.getCoordinate().first;
    // double y = delta.getCoordinate().second;
    return x; 
}

std::pair<double,double> Ball::getCentre_ball() const {
    return ball.getCentre();
}

/*void Ball::rebond_arena(){
    std::cout << "Work in progress" << std::endl;
}*/

/*void Ball::rebond_brick(const Brick& brick){
    std::cout << "Work in progress" << std::endl;
}*/

/*void Ball::rebond_paddle(const Paddle& paddle){
    std::cout << "Work in progress" << std::endl;
}*/

bool Ball::is_in_arena() const {
    double x = getCentre_ball().first;
    double y = getCentre_ball().second;
    return (x >= 0) && (x <= arena_size) && (y >= 0) && (y <= arena_size);
}

//--------------------------- Définition de la classe Brick ---------------------------

Brick::Brick(double x_, double y_, double length_, double width_,
             int hp_, char color_, bool is_destroyed_)
    : brick(x_, y_, length_, width_), color(color_), hp(hp_),
      is_destroyed(is_destroyed_) {
    brick_count++;
}

Brick::~Brick(){
    brick_count--;
}

Rectangle Brick::getRectangle() const {
    return Rectangle(brick);
}

/*void Brick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}

bool Brick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}

void Brick::destroy(){
    std::cout << "Work in progress" << std::endl;
}*/

//-------------------------- Définition de la classe RwBrick --------------------------

RwBrick::RwBrick(double x_, double y_, double length_, double width_, int hp_,
                 char color_, bool is_destroyed_, int current_color_i_)
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

/*void RwBrick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}

bool RwBrick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}

void RwBrick::destroy(){
    std::cout << "Work in progress" << std::endl;
}*/

//------------------------- Définition de la classe BallBrick -------------------------

BallBrick::BallBrick(double x_, double y_, double length_, double width_, 
                  char color_, bool is_destroyed_, 
                  double b_radius_, double dx_, double dy_, char b_color_, 
                  bool is_b_destroyed_)
         : Brick(0, 0, 0, 0, color_, is_destroyed_), ball_inside(x_, x_, b_radius_, 
                 dx_, dy_, b_color_, is_b_destroyed_){
        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_); 
        }

        brick_count++;
}

BallBrick::~BallBrick() {}

char BallBrick::getColor(){
    return color;
}

/*void BallBrick::on_hit(){
    std::cout << "Work in progress" << std::endl;
}

bool BallBrick::is_alive(){
    std::cout << "Work in progress" << std::endl;
}

void BallBrick::destroy(){
    std::cout << "Work in progress" << std::endl;
}*/

//------------------------- Définition de la classe SpltBrick -------------------------

SpltBrick::SpltBrick(double x_, double y_, double length_, double width_,
                char color_, bool is_destroyed_, int split_count_, int children_created_, 
                int current_color_i_)
          : Brick(0,0,0,0, color_, is_destroyed_), split_count(split_count_), 
            children_created(children_created_), current_color_i(current_color_i_){

        if ((width_ >= brick_size_min) && (length_ >= brick_size_min)){
            brick = Rectangle(x_, y_, length_, width_); 
        }

        current_color_i = floor(log2(length_/brick_size_min));
        brick_count++;
}

SpltBrick::~SpltBrick(){
    brick_count--;
}

char SpltBrick::getColor(){
    return colors[current_color_i];
}

/*void SpltBrick::on_hit(){
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
}*/


