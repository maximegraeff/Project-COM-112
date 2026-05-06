
// game.h : header du fichier game.cc, contenant les prototypes des fonctions 
//          de lecture et d'initialisation des objets du jeu.
//
//
// Version 3.4
//

#ifndef GAME_H
#define GAME_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <gtkmm/drawingarea.h>
#include "message.h"
#include "constants.h"
#include "bricks.h"
#include "ball.h"
#include "paddle.h"

using namespace std;
using namespace Gtk;

// Structure de données globale pour stocker les données du jeu
struct GameData {
        bool file_good = true;
        bool last_file_good = true;
        int score = 0;
        int lives = 0;
        int nb_brick = 0;
        int nb_ball = 0;
        unique_ptr<Paddle> paddle;
        vector<unique_ptr<Brick>> bricks;
        vector<unique_ptr<Ball>> balls;
};

// Enumération des objets à initialiser
enum ObjectType { SCORE, LIVES, PADDLE, BRICK, CO_BRICK, BALL, CO_BALL, END};

//---------------------------- Prototypes des fonctions -------------------------------

// Fonction qui lit le fichier et utilise les données
void read(string filename);

// Fonction de réinitialisation des données du jeu
void reset_();

// Fonction d'arret de lecture
void file_error();

// Fonction redirigeant vers la fonction d'initialisation de l'objet correspondant 
// à l'étape de lecture
void use_data(string line);

// Fonction de vérification et d'initialisation du score
void score_init(int score);

// Fonction de vérification et d'initialisation du nombre de lives
void lives_init(int lives);

// Fonction de vérification des données et d'initialisation du paddle
void paddle_init(string line);

// Fonction d'initialisation du nombre de bricks
void nb_brick_init(int brick_nb);

// Fonction de vérification des données et d'initialisation de la brick
void brick_init(string line);

// Fonction d'initialisation du nombre de balls
void nb_ball_init(int ball_nb);

// Fonction de vérification des données et d'initialisation de la ball
void ball_init(string line);

// Fonction de vérification des données de la brick
void is_brick_good(double x, double y, double size, int type, int hit_points);

// Fonction de vérification des données de la ball
void is_ball_good(double x, double y, double radius, double delta_x, double delta_y);

// Fonction de vérification de l'absence de collision entre la brick et les autres 
// objets
void intersects_rectangle(Rectangle r);

// Fonction de vérification de l'absence de collision entre la ball et les autres 
// objets
void intersects_circle(Circle c);

// Fonction d'initialisation de la brick en fonction de son type
void set_brick(double x, double y, double size, int type, int hit_points);

// Fonction d'écriture et de sauvegarde du fichier
void save_game(string& file_name);

void update_game(DrawingArea& drawing);

void update_paddle(DrawingArea& drawing);

pair<double, double> limit_delta(double dx, double dy);

double paddle_collision(double x, double temp_x, double y, double r, double dx);

void create_new_ball(DrawingArea& drawing);

bool new_ball_intersects(const Circle& new_ball);

void update_balls(DrawingArea& drawing);

pair<double, double> ball_collision(const unique_ptr<Ball>& ball, double dx, 
                                    double dy);

pair<double, double> ball_bricks_collision(double x_b, double y_b, double r, double dx,
                                           double dy, double x_brick, double y_brick, 
                                           double w);

pair<double, double> ball_paddle_collision(double dx, double dy);

pair<double, double> ball_circle_collision(double r, double dx, double dy, double r_c,
                                           double dx_c, double dy_c);


#endif