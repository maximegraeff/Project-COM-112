// read.cc  : # fonctions qui lisent le fichier passé en entrée, qui verifie si les données sont conformes aux 
//            spécificaitons et qui appelle les fonctions de init.cc pour créer les objets.            
//
//
// Version 1.0 du 05.03.2025
//
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "message.h"
#include "constants.h"
#include "read.h"


using namespace std;

static unsigned object(SCORE);
static string paddle("");
static int nb_brick(0);
static int brick_count(0);
static int nb_ball(0);
static int ball_count(0);

GameData data;

// Fonction qui lit le fichier et utilise les données
bool read(string filename)
{
    ifstream file(filename);

    if (!file.fail()) 
    {
        string line;

        while (getline(file >> ws, line)) {

            if(line[0]=='#') continue;

            use_data(line, data);
        
        }

        return true;
    }

    error(OPENING);
    return false;
}

// Fonction qui utilise les données lues pour vérifier leur validité et les stocker dans les variables globales
void use_data(string line, GameData& data)
{
    switch (object)
    {
        case SCORE: 
        {
            // Vérification de la validité du score et initialisation du score
            if (stoi(line) >= 0) data.score = stoi(line);    
            else cout << message::invalid_score(stoi(line)) << endl;   
            object = LIVES;
            break;
        }

        case LIVES:
        {
            
            // Vérification de la validité du score et initialisation du nombre de vies
            if (stoi(line) >= 0) data.lives = stoi(line); 
            else cout << message::invalid_lives(stoi(line)) << endl;
            object = PADDLE;
            break;
        }

        case PADDLE:
        {
            istringstream passor(line);
            double x, y, radius;
            passor >> x >> y >> radius;
            double width = sqrt(pow(radius, 2) - pow(y, 2));
            if (y + radius <= 0 or y > 0  or x - width < 0 or x + width > arena_size) cout << message::paddle_outside(x, y) << endl;    // Vérification de la position du paddle
            //data.paddle = Paddle(x, y, radius, ...);  // Initialisation du paddle);
            object = BRICK;
            break;
        }

        case BRICK:
        {
            nb_brick = stoi(line);
            object = CO_BRICK;
            break;
        }

        case CO_BRICK:
        {
            istringstream passor(line);
            double x, y, size;
            int type, hit_points;
            passor >> x >> y >> size >> type >> hit_points;

            // Vérification de la validité des données de la brick
            is_brick_good(x, y, size, type, hit_points);    
            Rectangle brick(x, y, size, size);
            intersects_rectangle(brick);
            // Initialisation de la brick
            data.bricks.push_back(make_unique<Brick>(brick, hit_points));  
            // Vérification du nombre de bricks et passage à la lecture des données des balls
            if (brick_count < nb_brick - 1) object = BALL;  
            break;
        }

        case BALL:
        {
            nb_ball = stoi(line);
            object = CO_BALL;
            break;
        }

        case CO_BALL:
        {
            istringstream passor(line);
            double x, y, radius, delta_x, delta_y;
            passor >> x >> y >> radius >> delta_x >> delta_y;

            // Vérification de la validité des données de la ball
            is_ball_good(x, y, radius, delta_x, delta_y);     
            Circle ball(x, y, radius);
            intersects_circle(ball);
            // Initialisation de la ball
            data.balls.push_back(Ball(ball, delta_x, delta_y));  
            break;
        }

        default:
            break;
    }

}

void error(ErrorCode code)
{ 
    switch(code)
    {
        case OPENING:
            cerr << "Erreur lors de l'ouverture du fichier." << endl;
            break;
        case READING:
            cerr << "Erreur lors de la lecture du fichier." << endl;
            break;
    }

}

void is_brick_good(double x, double y, double size, int type, int hit_points)
{   
    // Vérification de la validité de la taille de la brick
    if (size < brick_size_min) cout << message::invalid_brick_size(size) << endl;
    // Vérification de la validité du type de la brick
    else if (type < 0 or type > 2) cout << message::invalid_brick_type(type) << endl;       
    // Vérification de la validité du hit points de la brick
    else if (hit_points < 0 or hit_points > 7) cout << message::invalid_hit_points(hit_points) << endl;                                      
    // Vérification de la validité de la position de la brick
    else if (x - size < 0 or x + size > arena_size or y - size < 0 or y + size > arena_size) cout << message::brick_outside(x, y) << endl;   
}

void is_ball_good(double x, double y, double radius, double delta_x, double delta_y)
{
    // Vérification de la validité de la position de la ball
    if (x - radius < 0 or x + radius > arena_size or y - radius < 0 or y + radius > arena_size) cout << message::ball_outside(x, y) << endl;   
    // Vérification de la validité du delta de la ball
    else if (sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > delta_norm_max) cout << message::invalid_delta(delta_x, delta_y) << endl;        
}
void intersects_rectangle(Rectangle r)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(data.paddle.getCircle(), r)) {
        cout << message::collision_paddle_brick(brick_count + 1) << endl;
        return;
    }
    // Vérification de l'absence de collision avec les autres bricks 
    for (int j = 0; j < brick_count; j++) {
        if (intersects(r, data.bricks[j]->getRectangle())) {   
            cout << message::collision_bricks(brick_count + 1, j + 1) << endl;
                        return;
        }
    }
}

void intersects_circle(Circle c)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(c, data.paddle.getCircle())) {
        cout << message::collision_paddle_ball(ball_count + 1) << endl;
        return;
    }
    // Vérification de l'absence de collision avec les bricks 
    for (int j = 0; j < brick_count; j++) {
        if (intersects(c, data.bricks[j]->getRectangle())) {   
            cout << message::collision_ball_brick(ball_count + 1, j + 1) << endl;
                        return;
        }
    }
    for (int i = 0; i < ball_count; i++) {
        if (intersects(c, data.balls[i].getCircle())) {   
            cout << message::collision_balls(ball_count + 1, i + 1) << endl;
                        return;
        }
    }
}