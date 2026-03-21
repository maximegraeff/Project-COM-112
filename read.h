# ifndef READ_H
# define READ_H


#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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


// Enumération pour les codes d'erreur
enum ErrorCode { OPENING, READING };

enum ObjectType { SCORE, LIVES, PADDLE, BRICK, CO_BRICK, BALL, CO_BALL };

// Prototypes des fonctions
void error(ErrorCode code);
bool read(string filename);
void use_data(string line, GameData& data);

#endif