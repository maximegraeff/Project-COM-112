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
#include <ostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <memory>
#include <gtkmm/drawingarea.h>
#include "message.h"
#include "constants.h"
#include "game.h"
#include "bricks.h"
#include "ball.h"
#include "paddle.h"

using namespace std;
using namespace Gtk;

static unsigned object(SCORE);

GameData game_data;

// Lecture du fichier et utilisation des données
void read(string filename)
{
    // réinitialisation des données du jeu avant la lecture du fichier
    reset_(); 
    object = SCORE;
    // game_data = GameData{};

    ifstream file(filename);
    
    // Vérification que le fichier s'est bien ouvert
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        file_error();
        return;
    }
    
    string line;

    while (getline(file >> ws, line) and game_data.file_good) {

        if(line[0]=='#') continue;
        use_data(line);
        
    }

    // Données du fichier vérifiées et objets initialisés avec succès
    game_data.last_file_good = game_data.file_good;
    if (game_data.file_good) cout << message::success() << endl;
    else reset_();
}

void reset_()
{
    object = SCORE;
    game_data.file_good = true;
    game_data.score = 0;
    game_data.lives = 0;
    game_data.nb_brick = 0;
    game_data.nb_ball = 0;
    game_data.paddle.reset();
    game_data.bricks.clear();
    game_data.balls.clear();
}

void file_error()
{
    game_data.file_good = false;
}

// Redirection vers la fonction d'initialisation de l'objet correspondant 
// à l'étape de lecture
void use_data(string line)
{
    switch (object)
    {
        case SCORE: score_init(stoi(line)); break;

        case LIVES: lives_init(stoi(line)); break;

        case PADDLE: paddle_init(line); break;

        case BRICK: nb_brick_init(stoi(line)); break;

        case CO_BRICK: brick_init(line); break;

        case BALL: nb_ball_init(stoi(line)); break;

        case CO_BALL: ball_init(line); break;

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
        file_error();
    }
    object = LIVES;
}

// Vérification et initialisation du nombre de vies
void lives_init(int lives)
{
    if (lives >= 0) game_data.lives = lives;
    else {
        cout << message::invalid_lives(lives) << endl;   
        file_error();
    }
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
        file_error();
        return;
    }

    // Initialisation du paddle
    game_data.paddle = make_unique<Paddle>(x, y, radius, width); 
    object = BRICK;
}

// Initialisation du nombre de bricks
void nb_brick_init(int brick_nb)
{
    game_data.nb_brick = brick_nb;

    // vérification du nombre de bricks et passage à la lecture des données des bricks
    // ou du nombre de balls s'il n'y a pas de brick
    if (game_data.nb_brick == 0) object = BALL;
    else object = CO_BRICK;
}

// Vérification des données et initialisation de la brick
void brick_init(string line)
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
    intersects_rectangle(brick);

    // Initialisation de la brick
    set_brick(x, y, size, type, hit_points);

    // Vérification du nombre de bricks et passage à la lecture des données des balls 
    // s'il n'y a plus de brick à initialiser
    if (Brick::get_brick_count() == game_data.nb_brick) object = BALL;
}

// Initialisation du nombre de balls
void nb_ball_init(int ball_nb)
{
    game_data.nb_ball = ball_nb;

    // vérification du nombre de balls et passage à la lecture des données des balls
    // si le nombre de ball est supérieur à 0
    if (game_data.nb_ball == 0) object = END;
    else object = CO_BALL;
}

// Vérification des données de la ball et initialisation de la ball
void ball_init(string line)
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
    intersects_circle(ball);

    // Initialisation de la ball
    game_data.balls.push_back(make_unique<Ball>(x, y, radius, delta_x, delta_y));
}

//-------------------------- Fonctions de vérification --------------------------------

// Vérification des données de la brick
void is_brick_good(double x, double y, double size, int type, int hit_points)
{   
    // Vérification de la validité de la taille de la brick
    if (size < brick_size_min) {
        cout << message::invalid_brick_size(size) << endl;
        file_error();
        return;
    }
    // Vérification de la validité du type de la brick
    if (type < 0 or type > 2) {
        cout << message::invalid_brick_type(type) << endl;
        file_error();
        return;
    }

    // Vérification de la validité du hit points de la brick lorsque la brick est de 
    // type rainbow
    if (type ==0) {
        if (hit_points < 0 or hit_points > 7) {
        cout << message::invalid_hit_points(hit_points) << endl;
        file_error();
        return;
        }
    }

    // Vérification de la validité de la position de la brick
    if (!is_inside_arena(x, y, size, 0)) {
        cout << message::brick_outside(x, y) << endl;
        file_error();
        return;
    }
}

// Vérification des données de la ball
void is_ball_good(double x, double y, double radius, double delta_x, double delta_y)
{
    // Vérification de la validité de la position de la ball
    if (!is_inside_arena(x, y, 2*radius, 0)) {
        cout << message::ball_outside(x, y) << endl;
        file_error();
    }

    // Vérification de la validité du delta de la ball
    else if (sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > delta_norm_max) {
        cout << message::invalid_delta(delta_x, delta_y) << endl;
        file_error();
    }
}

// Vérification de l'absence de collision entre la brick et les autres objets 
// déjà initialisés
void intersects_rectangle(Rectangle r)
{   
    // Vérification de l'absence de collision avec le paddle
    if (game_data.paddle and intersects(game_data.paddle->getCircle(), r, 0)) {
        cout << message::collision_paddle_brick(Brick::get_brick_count()) << endl;
        file_error();
    }
    // Vérification de l'absence de collision avec les autres bricks 
    for (size_t j = 0; j < game_data.bricks.size(); j++) {
        if (intersects(r, game_data.bricks[j]->getRectangle(), 0)) {   
            cout << message::collision_bricks(Brick::get_brick_count(), j) << endl;
            file_error();
        }
    }
}

// Vérification de l'absence de collision entre la ball et les autres objets 
// déjà initialisés
void intersects_circle(Circle c)
{   
    // Vérification de l'absence de collision avec le paddle
    if (game_data.paddle and intersects(c, game_data.paddle->getCircle(), 0)) {
        cout << message::collision_paddle_ball(Ball::get_ball_count()) << endl;
        file_error();
    }
    // Vérification de l'absence de collision avec les bricks 
    for (size_t j = 0; j < game_data.bricks.size(); j++) {
        if (intersects(c, game_data.bricks[j]->getRectangle(), 0)) {   
            cout << message::collision_ball_brick(Ball::get_ball_count(), j) << endl;
            file_error();
        }
    }
    // Vérification de l'absence de collision avec les autres balls
    for (size_t i = 0; i < game_data.balls.size(); i++) {
        if (intersects(c, game_data.balls[i]->getCircle(), 0)) {   
            cout << message::collision_balls(Ball::get_ball_count(), i) << endl;
            file_error();
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


//---------------------------- Fonction de sauvegrade ---------------------------------

// Ecriture d'un fichier pour la sauvegarde du jeu

void save_game(string& file_name){

    ofstream out(file_name);

    if (!out){
            cout << "Error: Unable to open the backup file" << endl;
    }
    else{
        // Score et lives
        out << "# score\n" << game_data.score << "\n";
        out << "# lives\n" << game_data.lives << "\n";

        // Paddle
        out << "# paddle\n";
        if (game_data.paddle)
        {
            const Circle paddle_circle = game_data.paddle->getCircle();
            const auto paddle_center = paddle_circle.getCentre();
            out << paddle_center.first << " " << paddle_center.second << " "
                        << paddle_circle.getRadius() << "\n";
        }

        // Briques
        out << "# bricks\n";
        out << Brick::get_brick_count() << "\n";
        for (const auto& b : game_data.bricks) {
            const Rectangle brick_rect = b->getRectangle();
            const auto brick_center = brick_rect.getCentre();
            out << b->getType() << " " << brick_center.first << " "
                        << brick_center.second << " " << brick_rect.getLength();
            if (b->getType() == 0)
                out << " " << b->getHitPoints();
                out << "\n";
        }

        // Balles
        out << "# balls\n";
        out << Ball::get_ball_count() << "\n";
        for (const auto& b : game_data.balls) {
            const Circle ball_circle = b->getCircle();
            const auto ball_center = ball_circle.getCentre();
            const auto delta = b->getDeltaVector();
            out << ball_center.first << " " << ball_center.second << " "
                        << ball_circle.getRadius() << " " << delta.first << " "
                        << delta.second << "\n";
        } 
    }
}

void update_game(DrawingArea& drawing) {
    update_paddle(drawing);
}

void update_paddle(DrawingArea& drawing) {   
    if (game_data.paddle) {
        double x_t = game_data.paddle->get_target_x();
        double x = game_data.paddle->getCenter_paddle().first;
        double y = game_data.paddle->getCenter_paddle().second;
        double w = game_data.paddle->getWidth();
        double r = game_data.paddle->getCircle().getRadius();
    
        x_t = max(w, min(arena_size - w, x_t));

        double dx = x_t - x;
        dx = limit_delta(dx, 0).first;
        double temp_x = x + dx;
        // x le plus proche possible de x_t sans collision avec les briques
        double new_x = paddle_collision(x, temp_x, y, r, dx);

        if (new_x != x) {
            game_data.paddle->setCentrePaddle(new_x, y);
            drawing.queue_draw();
        }
    }
}

// Limite la norme du vecteur (dx, dy) à delta_norm_max
pair<double, double> limit_delta(double dx, double dy) {
    if (abs(dx) > delta_norm_max) dx = delta_norm_max*dx/abs(dx);
    if (abs(dy) > delta_norm_max) dy = delta_norm_max*dy/abs(dy);
    return {dx, dy};
}

double paddle_collision(double x, double temp_x, double y, double r, 
                                   double dx) {
    
    Circle new_paddle_circle(temp_x, y, r);
    bool collision = false;
    for (const auto& brick : game_data.bricks) {
        if (brick and intersects(new_paddle_circle, brick->getRectangle())) {
            collision = true;
            break;
        }
    }

    if (collision) {
        double step = 0;
        if (dx > 0) step = 0.1;
        else step = -0.1;
        double best_x = x;
        double test_x = x + step;
        while (abs(test_x - x) <= abs(dx)) {
            Circle test_circle(test_x, y, r);
            bool hit = false;
            for (const auto& brick : game_data.bricks) {
                if (brick and intersects(test_circle, brick->getRectangle())) {
                    hit = true;
                    break;
                }
            }
            if (hit) break;
            best_x = test_x;
            test_x += step;
        }
        return best_x;
    }
    else {
        return temp_x;
    }
}