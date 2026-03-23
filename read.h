# ifndef READ_H
# define READ_H


#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include "message.h"
#include "constants.h"
#include "objects.h"

using namespace std;


struct GameData {
        int score;
        int lives;
        int nb_brick;
        int nb_ball;
        int brick_count;
        int ball_count;
        unique_ptr<Paddle> paddle; // Initialisation du paddle avec des valeurs par défaut
        vector<unique_ptr<Brick>> bricks;
        vector<unique_ptr<Ball>> balls;
};

enum ObjectType { SCORE, LIVES, PADDLE, BRICK, CO_BRICK, BALL, CO_BALL };

// Prototypes des fonctions
void read(string filename);
void score_init(int score);
void lives_init(int lives);
void paddle_init(string line);
void nb_brick_init(int brick_nb);
void brick_init(string line);
void nb_ball_init(int ball_nb);
void ball_init(string line);
void use_data(string line, GameData& data);
void is_brick_good(double x, double y, double size, int type, int hit_points);
void is_ball_good(double x, double y, double radius, double delta_x, double delta_y);
void intersects_rectangle(Rectangle r);
void intersects_circle(Circle c);
void set_brick(Rectangle brick, int type, int hit_points);

#endif