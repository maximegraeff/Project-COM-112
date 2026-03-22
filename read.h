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
        Paddle paddle;
        vector<unique_ptr<Brick>> bricks;
        vector<Ball> balls;
};


// Enumération pour les codes d'erreur (à dégager)
enum ErrorCode { OPENING, READING };

enum ObjectType { SCORE, LIVES, PADDLE, BRICK, CO_BRICK, BALL, CO_BALL };

// Prototypes des fonctions
void error(ErrorCode code); // à dégager
bool read(string filename);
void use_data(string line, GameData& data);
void is_brick_good(double x, double y, double size, int type, int hit_points);
void is_ball_good(double x, double y, double radius, double delta_x, double delta_y);
void intersects_rectangle(Rectangle r);
void intersects_circle(Circle c);

#endif