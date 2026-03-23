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
#include "objects.h"


using namespace std;

static unsigned object(SCORE);

GameData data;

// Fonction qui lit le fichier et utilise les données
void read(string filename)
{
    ifstream file(filename);
    string line;

    while (getline(file >> ws, line)) {

        if(line[0]=='#') continue;
        use_data(line, data);
        
    }

    cout << message::success() << endl;
}

// Fonction qui utilise les données lues pour vérifier leur validité et les stocker dans les variables globales
void use_data(string line, GameData& data)
{
    switch (object)
    {
        case SCORE: 
        {
            // Vérification de la validité du score et initialisation du score
            score_init(stoi(line));
            break;
        }

        case LIVES:
        {
            // Vérification de la validité du score et initialisation du nombre de vies
            lives_init(stoi(line));
            break;
        }

        case PADDLE:
        {
            // Vérification de la validité des données du paddle et initialisation du paddle
            paddle_init(line);
            break;
        }

        case BRICK:
        {
            // Initialisation du nombre de bricks
            nb_brick_init(stoi(line), data);
            break;
        }

        case CO_BRICK:
        {
            // Vérification de la validité des données de la brick et initialisation de la brick
            brick_init(line, data);
            break;
        }

        case BALL:
        {
            // Initialisation du nombre de balls
            nb_ball_init(stoi(line), data);
            break;
        }

        case CO_BALL:
        {
            // Vérification de la validité des données de la ball et initialisation de la ball
            ball_init(line, data);
            break;
        }

        default:
            break;
    }

}

void score_init(int score)
{
    if (score >= 0) data.score = score;    
    else {
        cout << message::invalid_score(score) << endl;   
        exit(0);
    }
    object = LIVES;
}

void lives_init(int lives)
{
    if (lives >= 0) data.lives = lives;    
    else {
        cout << message::invalid_lives(lives) << endl;   
        exit(0);
    }
    object = PADDLE;
}

void paddle_init(string line)
{
    istringstream passor(line);
    double x, y, radius;
    passor >> x >> y >> radius;
    double width = sqrt(pow(radius, 2) - pow(y, 2));

    // Vérification de la position du paddle
    if (y + radius <= 0 or y > 0  or x - width < 0 or x + width > arena_size) cout << message::paddle_outside(x, y) << endl;
    // Initialisation du paddle
    data.paddle = make_unique<Paddle>(x, y, radius);    
    object = BRICK;
}

void nb_brick_init(int brick_nb, GameData& data)
{
    data.nb_brick = brick_nb;      
    object = CO_BRICK;
}

void brick_init(string line, GameData& data)
{
    istringstream passor(line);
    double x, y, size;
    int type, hit_points;
    passor >> x >> y >> size >> type >> hit_points;

    // Vérification de la validité des données de la brick
    is_brick_good(x, y, size, type, hit_points);    
    Rectangle brick(x, y, size, size);
    intersects_rectangle(brick, data);

    // Initialisation de la brick
    set_brick(brick, type, hit_points);  

    // Vérification du nombre de bricks et passage à la lecture des données des balls
    if (data.brick_count < data.nb_brick - 1) object = BALL;  
}

void nb_ball_init(int ball_nb, GameData& data)
{
    data.nb_ball = ball_nb;      
    object = CO_BALL;
}

void ball_init(string line, GameData& data)
{
    istringstream passor(line);
    double x, y, radius, delta_x, delta_y;
    passor >> x >> y >> radius >> delta_x >> delta_y;

    // Vérification de la validité des données de la ball
    is_ball_good(x, y, radius, delta_x, delta_y);     
    Circle ball(x, y, radius);
    intersects_circle(ball, data);

    // Initialisation de la ball
    data.balls.push_back(make_unique<Ball>(ball, delta_x, delta_y));
}

void is_brick_good(double x, double y, double size, int type, int hit_points)
{   
    // Vérification de la validité de la taille de la brick
    if (size < brick_size_min) {
        cout << message::invalid_brick_size(size) << endl;
        exit(0);
    }
    // Vérification de la validité du type de la brick
    else if (type < 0 or type > 2) {
        cout << message::invalid_brick_type(type) << endl;
        exit(0);
    }

    // Vérification de la validité du hit points de la brick
    else if (hit_points < 0 or hit_points > 7) {
        cout << message::invalid_hit_points(hit_points) << endl;
        exit(0);
    }

    // Vérification de la validité de la position de la brick
    else if (x - size < 0 or x + size > arena_size or y - size < 0 or y + size > arena_size) {
        cout << message::brick_outside(x, y) << endl;
        exit(0);
    }
}

void is_ball_good(double x, double y, double radius, double delta_x, double delta_y)
{
    // Vérification de la validité de la position de la ball
    if (x - radius < 0 or x + radius > arena_size or y - radius < 0 or y + radius > arena_size) {
        cout << message::ball_outside(x, y) << endl;
        exit(0);
    }

    // Vérification de la validité du delta de la ball
    else if (sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > delta_norm_max) {
        cout << message::invalid_delta(delta_x, delta_y) << endl;
        exit(0);
    }
}
void intersects_rectangle(Rectangle r, GameData& data)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(data.paddle->getCircle(), r)) {
        cout << message::collision_paddle_brick(data.brick_count + 1) << endl;
        exit(0);
    }
    // Vérification de l'absence de collision avec les autres bricks 
    for (int j = 0; j < data.brick_count; j++) {
        if (intersects(r, data.bricks[j]->getRectangle())) {   
            cout << message::collision_bricks(data.brick_count + 1, j + 1) << endl;
            exit(0);
        }
    }
}

void intersects_circle(Circle c, GameData& data)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(c, data.paddle->getCircle())) {
        cout << message::collision_paddle_ball(data.ball_count + 1) << endl;
        exit(0);
    }
    // Vérification de l'absence de collision avec les bricks 
    for (int j = 0; j < data.brick_count; j++) {
        if (intersects(c, data.bricks[j]->getRectangle())) {   
            cout << message::collision_ball_brick(data.ball_count + 1, j + 1) << endl;
            exit(0);
        }
    }
    for (int i = 0; i < data.ball_count; i++) {
        if (intersects(c, data.balls[i]->getCircle())) {   
            cout << message::collision_balls(data.ball_count + 1, i + 1) << endl;
            exit(0);
        }
    }
}

void set_brick(Rectangle brick, int type, int hit_points)  {
    if (type == 0)data.bricks.push_back(make_unique<RwBrick>(brick, hit_points));
    else if (type == 1) data.bricks.push_back(make_unique<BallBrick>(brick, hit_points));
    else if (type == 2) data.bricks.push_back(make_unique<SpltBrick>(brick, hit_points));
}