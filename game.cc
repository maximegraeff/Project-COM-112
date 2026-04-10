// game.cc  : # fonctions qui lisent le fichier passé en entrée, qui verifie si 
//            les données sont conformes aux spécificaitons et qui appelle 
//            les fonctions de objects.cc pour créer les objets.            
//
//
// Version 3.4
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
#include "game.h"
#include "bricks.h"
#include "ball.h"
#include "paddle.h"

using namespace std;

static unsigned object(SCORE);

GameData game_data;

// Lecture du fichier et utilisation des données
void read(string filename)
{
    cerr << "score: " << game_data.score << endl;
    cerr << "lives: " << game_data.lives << endl;
    cerr << "nb_brick: " << game_data.nb_brick << endl;
    cerr << "nb_ball: " << game_data.nb_ball << endl;
    cerr << "brick_count: " << game_data.brick_count << endl;
    cerr << "ball_count: " << game_data.ball_count << endl;
    cerr << "bricks size: " << game_data.bricks.size() << endl;
    cerr << "balls size: " << game_data.balls.size() << endl;

    ifstream file(filename);
    string line;

    while (getline(file >> ws, line)) {

        if(line[0]=='#') continue;
        use_data(line, game_data);
        
    }

    // Données du fichier vérifiées et objets initialisés avec succès
    cout << message::success() << endl;
}

// Redirection vers la fonction d'initialisation de l'objet correspondant 
// à l'étape de lecture
void use_data(string line, GameData& game_data)
{
    switch (object)
    {
        case SCORE: score_init(stoi(line)); break;

        case LIVES: lives_init(stoi(line)); break;

        case PADDLE: paddle_init(line); break;

        case BRICK: nb_brick_init(stoi(line), game_data); break;

        case CO_BRICK: brick_init(line, game_data); break;

        case BALL: nb_ball_init(stoi(line), game_data); break;

        case CO_BALL: ball_init(line, game_data); break;

        case END: break;

        default: break;
    }

}

//-------------------------- Functions d'initialisation -------------------------------

// Vérification et initialisation du score
void score_init(int score)
{
    if (score >= 0) game_data.score = score;
    else {
        cout << message::invalid_score(score) << endl;   
        exit(0);
    }
    object = LIVES;
}

// Vérification et initialisation du nombre de vies
void lives_init(int lives)
{
    if (lives >= 0) game_data.lives = lives;
    else {
        cout << message::invalid_lives(lives) << endl;   
        exit(0);
    }
    cout << "lives " << lives << endl;
    object = PADDLE;
}

// Vérification des données et initialisation du paddle
void paddle_init(string line)
{
    istringstream passor(line);
    double x, y, radius;
    passor >> x >> y >> radius;
    double width = sqrt(pow(radius, 2) - pow(y, 2));

    // Vérification de la position du paddle
    if (y + radius <= 0 or y > 0 or x - width < 0 or x + width > arena_size)    {
        cout << message::paddle_outside(x, y) << endl;
        exit(0);
    }

    // Initialisation du paddle
    game_data.paddle = make_unique<Paddle>(x, y, radius); 
    object = BRICK;
}

// Initialisation du nombre de bricks
void nb_brick_init(int brick_nb, GameData& game_data)
{
    game_data.nb_brick = brick_nb;

    // vérification du nombre de bricks et passage à la lecture des données des bricks
    // ou du nombre de balls s'il n'y a pas de brick
    if (game_data.nb_brick == 0) object = BALL;
    else object = CO_BRICK;
}

// Vérification des données et initialisation de la brick
void brick_init(string line, GameData& game_data)
{   
    istringstream passor(line);
    double x, y, size;
    int type, hit_points;
    passor >> type >> x >> y >> size >> hit_points;

    // Vérification des données
    is_brick_good(x, y, size, type, hit_points);
    
    // Création d'un rectangle correspondant à la brick pour vérifier les collisions
    // avec les autres objets déjà initialisés
    Rectangle brick(x, y, size, size);

    // Vérification des colisions
    intersects_rectangle(brick, game_data);

    // Initialisation de la brick
    set_brick(x, y, size, type, hit_points);

    // Vérification du nombre de bricks et passage à la lecture des données des balls 
    // s'il n'y a plus de brick à initialiser
    game_data.brick_count++;
    if (game_data.brick_count == game_data.nb_brick) object = BALL;
}

// Initialisation du nombre de balls
void nb_ball_init(int ball_nb, GameData& game_data)
{
    game_data.nb_ball = ball_nb;

    // vérification du nombre de balls et passage à la lecture des données des balls
    // si le nombre de ball est supérieur à 0
    if (game_data.nb_ball == 0) object = END;
    else object = CO_BALL;
}

// Vérification des données de la ball et initialisation de la ball
void ball_init(string line, GameData& game_data)
{
    istringstream passor(line);
    double x, y, radius, delta_x, delta_y;
    passor >> x >> y >> radius >> delta_x >> delta_y;

    // Vérification des données
    is_ball_good(x, y, radius, delta_x, delta_y);  

    // Création d'un cercle correspondant à la ball pour vérifier les collisions
    // avec les autres objets déjà initialisés
    Circle ball(x, y, radius);

    // Vérification des colisions
    intersects_circle(ball, game_data);

    // Initialisation de la ball
    game_data.balls.push_back(make_unique<Ball>(x, y, radius, delta_x, delta_y));
    game_data.ball_count++;
}

//-------------------------- Fonctions de vérification --------------------------------

// Vérification des données de la brick
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

    // Vérification de la validité du hit points de la brick lorsque la brick est de 
    // type rainbow
    else if (type ==0) {
        if (hit_points < 0 or hit_points > 7) {
        cout << message::invalid_hit_points(hit_points) << endl;
        exit(0);
        }
    }

    // Vérification de la validité de la position de la brick
    else if (x - size/2 < 0 or x + size/2 > arena_size or y - size/2 < 0 
        or y + size/2 > arena_size) {
        cout << message::brick_outside(x, y) << endl;
        exit(0);
    }
}

// Vérification des données de la ball
void is_ball_good(double x, double y, double radius, double delta_x, double delta_y)
{
    // Vérification de la validité de la position de la ball
    if (x - radius < 0 or x + radius > arena_size or y - radius < 0 
        or y + radius > arena_size) {
        cout << message::ball_outside(x, y) << endl;
        exit(0);
    }

    // Vérification de la validité du delta de la ball
    else if (sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > delta_norm_max) {
        cout << message::invalid_delta(delta_x, delta_y) << endl;
        exit(0);
    }
}

// Vérification de l'absence de collision entre la brick et les autres objets 
// déjà initialisés
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

// Vérification de l'absence de collision entre la ball et les autres objets 
// déjà initialisés
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
    // Vérification de l'absence de collision avec les autres balls
    for (int i = 0; i < game_data.ball_count; i++) {
        if (intersects(c, game_data.balls[i]->getCircle())) {   
            cout << message::collision_balls(game_data.ball_count, i) << endl;
            exit(0);
        }
    }
}

//-------------------------- Fonction d'initialisation --------------------------------

// Initialisation de la brick en fonction de son type
void set_brick(double x, double y, double size, int type, int hit_points) {
    
    // Rainbow brick
    if (type == 0)
        game_data.bricks.push_back(make_unique<RwBrick>(x, y, size, size, hit_points));
    
    // Ball brick
    else if (type == 1)
        game_data.bricks.push_back(make_unique<BallBrick>(x, y, size, size)); 
    
    // Split brick
    else if (type == 2) 
        game_data.bricks.push_back(make_unique<SpltBrick>(x, y, size, size));
}
