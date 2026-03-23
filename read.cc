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
#include <memory>
#include "message.h"
#include "constants.h"
#include "read.h"
#include "objects.h"

using namespace std;

static unsigned object(SCORE);

GameData game_data;

// Fonction qui lit le fichier et utilise les données
void read(string filename)
{
    ifstream file(filename);
    string line;

    while (getline(file >> ws, line)) {

        if(line[0]=='#') continue;
        use_data(line, game_data);
        
    }

    cout << message::success() << endl;
}

// Fonction qui utilise les données lues pour vérifier leur validité et les stocker dans les variables globales
void use_data(string line, GameData& game_data)
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
            nb_brick_init(stoi(line), game_data);
            break;
        }

        case CO_BRICK:
        {
            // Vérification de la validité des données de la brick et initialisation de la brick
            brick_init(line, game_data);
            break;
        }

        case BALL:
        {
            // Initialisation du nombre de balls
            nb_ball_init(stoi(line), game_data);
            break;
        }

        case CO_BALL:
        {
            // Vérification de la validité des données de la ball et initialisation de la ball
            ball_init(line, game_data);
            break;
        }

        case END:
            break;

        default:
            break;
    }

}

void score_init(int score)
{
    if (score >= 0) {game_data.score = score; cout << game_data.score << endl;    }     ///////////////////
    else {
        cout << message::invalid_score(score) << endl;   
        exit(0);
    }
    object = LIVES;
}

void lives_init(int lives)
{
    if (lives >= 0) {game_data.lives = lives; cout << game_data.lives << endl;  }   ////////////////
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
    if (y + radius <= 0 or y > 0 or x - width < 0 or x + width > arena_size){
        cout << message::paddle_outside(x, y) << endl;
        exit(0);
    }
    game_data.paddle = make_unique<Paddle>(x, y, radius); 
    cout << game_data.paddle->getCenter_paddle().first << ", " << game_data.paddle->getCenter_paddle().second << endl;  /////////////////////////
    object = BRICK;
}

void nb_brick_init(int brick_nb, GameData& game_data)
{
    game_data.nb_brick = brick_nb;
    cout << "Number of bricks: " << game_data.nb_brick << endl;    ///////////////////
    if (game_data.nb_brick == 0) object = BALL;
    else object = CO_BRICK;
}

void brick_init(string line, GameData& game_data)
{   
    istringstream passor(line);
    double x, y, size;
    int type, hit_points;
    passor >> type >> x >> y >> size >> hit_points;

    // Vérification de la validité des données de la brick
    is_brick_good(x, y, size, type, hit_points);    
    Rectangle brick(x, y, size, size);
    intersects_rectangle(brick, game_data);

    // Initialisation de la brick
    set_brick(x, y, size, type, hit_points);

    // Vérification du nombre de bricks et passage à la lecture des données des balls
    game_data.brick_count++;
    if (game_data.brick_count == game_data.nb_brick) object = BALL;
}

void nb_ball_init(int ball_nb, GameData& game_data)
{
    game_data.nb_ball = ball_nb;
    cout << "Number of balls: " << game_data.nb_ball << endl;  /////////////////////
    if (game_data.nb_ball == 0) object = END;
    else object = CO_BALL;
}

void ball_init(string line, GameData& game_data)
{
    istringstream passor(line);
    double x, y, radius, delta_x, delta_y;
    passor >> x >> y >> radius >> delta_x >> delta_y;

    // Vérification de la validité des données de la ball
    is_ball_good(x, y, radius, delta_x, delta_y);     
    Circle ball(x, y, radius);
    intersects_circle(ball, game_data);

    // Initialisation de la ball
    game_data.balls.push_back(make_unique<Ball>(x, y, radius, delta_x, delta_y));
    cout << "Ball created : " << "x : "<< x << ", " << "y : " << y << ", " << "radius : " << radius << ", " << "delta_x : " << delta_x << ", " << "delta_y : " << delta_y << endl;  ////////////////////////
    game_data.ball_count++;
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
    else if (type ==0) {
        if (hit_points < 0 or hit_points > 7) {
        cout << message::invalid_hit_points(hit_points) << endl;
        exit(0);
        }
    }

    // Vérification de la validité de la position de la brick
    else if (x - size/2 < 0 or x + size/2 > arena_size or y - size/2 < 0 or y + size/2 > arena_size) {
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
void intersects_rectangle(Rectangle r, GameData& game_data)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(game_data.paddle->getCircle(), r)) {
        cout << message::collision_paddle_brick(game_data.brick_count) << endl;
        exit(0);
    }
    // Vérification de l'absence de collision avec les autres bricks 
    for (int j = 0; j < game_data.brick_count; j++) {
        if (intersects(r, game_data.bricks[j]->getRectangle())) {   
            cout << message::collision_bricks(game_data.brick_count, j) << endl;
            exit(0);
        }
    }
}

void intersects_circle(Circle c, GameData& game_data)
{   
    // Vérification de l'absence de collision avec le paddle
    if (intersects(c, game_data.paddle->getCircle())) {
        cout << message::collision_paddle_ball(game_data.ball_count) << endl;
        exit(0);
    }
    // Vérification de l'absence de collision avec les bricks 
    for (int j = 0; j < game_data.brick_count; j++) {
        if (intersects(c, game_data.bricks[j]->getRectangle())) {   
            cout << message::collision_ball_brick(game_data.ball_count, j) << endl;
            exit(0);
        }
    }
    for (int i = 0; i < game_data.ball_count; i++) {
        if (intersects(c, game_data.balls[i]->getCircle())) {   
            cout << message::collision_balls(game_data.ball_count, i) << endl;
            exit(0);
        }
    }
}

void set_brick(double x, double y, double size, int type, int hit_points) {
    if (type == 0) {game_data.bricks.push_back(make_unique<RwBrick>(x, y, size, size, hit_points)); cout << "RwBrick created : " << "x : "<< x << ", " << "y : " << y << ", " << "size : " << size << ", " << "hp : " << hit_points << endl;} ////////////////////////
    else if (type == 1) {game_data.bricks.push_back(make_unique<BallBrick>(x, y, size, size)); cout << "BallBrick created : " << "x : "<< x << ", " << "y : " << y << ", " << "size : " << size << endl;}  ////////////////////////
    else if (type == 2) {game_data.bricks.push_back(make_unique<SpltBrick>(x, y, size, size)); cout << "SpltBrick created : " << "x : "<< x << ", " << "y : " << y << ", " << "size : " << size << endl;}  ////////////////////////
}
