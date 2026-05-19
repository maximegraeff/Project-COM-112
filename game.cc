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
    game_data.game_ended = false;
    game_data.score = 0;
    game_data.lives = 0;
    game_data.nb_brick = 0;
    game_data.nb_ball = 0;
    game_data.paddle.reset();
    game_data.bricks.clear();
    game_data.balls.clear();
    game_data.new_balls.clear();
    game_data.new_bricks.clear();
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
    // Passe 1 : rebonds briques/bords/balles
    for (auto ball = game_data.balls.begin(); ball != game_data.balls.end(); ) {
        if ((*ball)->final_circle().getCentre().second < 0) {
            ball = game_data.balls.erase(ball);
            continue;  // pas de break, on continue avec les autres
        }
        collision(*ball);
        ++ball;
    }

    // Déplacement raquette
    update_paddle(drawing);

    // Passe 2 : rebond raquette (sans compter) puis rebonds suivants
    for (auto& ball : game_data.balls) {
        if (intersects(ball->final_circle(), game_data.paddle->getCircle())) {
            ball_paddle_collision(ball);  // pas de add_bounce()
            collision(ball);
        }
        ball->update_position();
        ball->reset_bounces();
    }

    new_components();
    drawing.queue_draw();
    // cout << "_____" << endl;
}

// void update_game(DrawingArea& drawing) {
//     first_balls_tests(drawing);
//     update_paddle(drawing);
//     second_balls_tests(drawing);
// }

// void first_balls_tests(DrawingArea& drawing) {
//     if (game_data.balls.empty()) return;
//     auto ball = game_data.balls.begin();
//     while (ball != game_data.balls.end()) {
//         if (*ball) {
//             double x = (*ball)->getCentre_ball().first;
//             double y = (*ball)->getCentre_ball().second;

//             // destruction de la balle si elle passe sous l'arène
//             if ((*ball)->final_circle().getCentre().second < 0) {
//                 ball = game_data.balls.erase(ball);
//                 break;
//             }
//             collision(*ball, x, y);
//             // cout << "test 1" << endl;
//         }
//         ++ball;
//     }
//     new_components();
//     drawing.queue_draw();
// }

// void second_balls_tests(DrawingArea& drawing) {
//     if (game_data.balls.empty()) return;
//     auto ball = game_data.balls.begin();
//     while (ball != game_data.balls.end()) {
//         if (*ball) {
//             double x = (*ball)->getCentre_ball().first;
//             double y = (*ball)->getCentre_ball().second;

//             if (intersects((*ball)->final_circle(), game_data.paddle->getCircle())) {
//                 ball_paddle_collision(*ball);
//                 (*ball)->update_delta();
//             }
//             collision(*ball, x, y);
//             // cout << "test 2" << endl;
//         }
//         ++ball;
//     }
//     for (const auto& ball : game_data.balls) {
//         ball->update_position();
//         ball->reset_bounces();
//     }
//     new_components();
//     drawing.queue_draw();
// }

void collision(const unique_ptr<Ball>& ball) {
    while(bounce_balls(ball)) {
        if (!ball->bounce()) break;
    }
}

bool bounce_balls(const unique_ptr<Ball>& ball) {

    if (ball->final_circle().getCentre().first - ball->getCircle().getRadius() < 0 or 
        ball->final_circle().getCentre().first + ball->getCircle().getRadius() > arena_size or
        ball->final_circle().getCentre().second + ball->getCircle().getRadius() > arena_size) {
        if (ball->bounce()) {
            ball_arena_collision(ball);
            ball->add_bounce();
        }
        return true;
    }

    for (const auto& ball_ : game_data.balls) {
        if (ball_ != ball and intersects(ball->final_circle(), ball_->final_circle())) {
            ball_circle_collision(ball, ball_);
            return true;
        }
        return true;
    }

    for (const auto& ball_ : game_data.balls) {
        if (ball_ != ball and intersects(ball->final_circle(), ball_->final_circle())) {
            ball_circle_collision(ball, ball_);
            return true;
        }
    }

    // for (int i = 0; i < game_data.balls.size(); i++){
    //     for (int j = i+1; j < game_data.balls.size(); j++) {

    //         auto& ball_i = game_data.balls[i];
    //         auto& ball_j = game_data.balls[j];

    //         double dx_i = ball_i->getDeltaVector().first;
    //         double dy_i = ball_i->getDeltaVector().second;
    //         double x_i = ball_i->getCentre_ball().first;
    //         double y_i = ball_i->getCentre_ball().second;
    //         double r_i = ball_i->getCircle().getRadius();

    //         double dx_j = ball_j->getDeltaVector().first;
    //         double dy_j = ball_j->getDeltaVector().second;
    //         double x_j = ball_j->getCentre_ball().first;
    //         double y_j = ball_j->getCentre_ball().second;
    //         double r_j = ball_j->getCircle().getRadius();

    //         double dist = sqrt(pow(x_i - x_j, 2) + pow(y_i - y_j, 2));

    //         //Distance normale entre les deux centres (checker la proximité des balles)
    //         double delta_norm_x = (x_j - x_i) / dist;
    //         double delta_norm_y = (y_j - y_i) / dist;

    //         // Vistesse relative selon la normale(chekcer si elles s'éloignent ou se 
    //         // rapprochent l'une de lautre)
    //         double v_relativ = (dx_i - dx_j)*delta_norm_x + (dy_i - dy_j)*delta_norm_y;

    //         if (dist < r_i + r_j && v_relativ > 0){
    //             // double overlap = (r_i + r_j - dist) / 2.0;
    
    //             // double dx_i_old = ball_i->getDeltaVector().first;
    //             // double dy_i_old = ball_i->getDeltaVector().second;
    //             // ball_i->setDeltaVector(-overlap * delta_norm_x, -overlap * delta_norm_y);
    //             // ball_i->update_position();
    //             // ball_i->setDeltaVector(dx_i_old, dy_i_old);
                
    //             // double dx_j_old = ball_j->getDeltaVector().first;
    //             // double dy_j_old = ball_j->getDeltaVector().second;
    //             // ball_j->setDeltaVector(overlap * delta_norm_x, overlap * delta_norm_y);
    //             // ball_j->update_position();
    //             // ball_j->setDeltaVector(dx_j_old, dy_j_old);


    //             if (ball == ball_i && ball_i->bounce()){
    //                 ball_circle_collision(ball_i, ball_j);
    //                 ball_i->add_bounce();
    //                 // cout << "test_bounce" << endl;
    //                 return true;
    //             }
    //             if (ball == ball_j && ball_j->bounce()){
    //                 ball_circle_collision(ball_j, ball_i);
    //                 ball_j->add_bounce();
    //                 // cout << "test_bounce" << endl;
    //                 return true;
    //             }
    //         }
    //     }
    // }

    for (const auto& brick : game_data.bricks) {
        if (brick and intersects(ball->final_circle(), brick->getRectangle())) {
            if (ball->bounce()) {
                ball_bricks_collision(ball, brick);
                ball->add_bounce();
            }
        return true;
        }
    }
    if (game_data.paddle and intersects(ball->final_circle(), game_data.paddle->getCircle())) {
        if (ball->bounce()) {
            ball_paddle_collision(ball);
            // ball->add_bounce();
        }
        return true;
    }
    return false;
}

void ball_arena_collision(const unique_ptr<Ball>& ball) {
    double dx = ball->getDeltaVector().first;
    double dy = ball->getDeltaVector().second;
    double x = ball->final_circle().getCentre().first;
    double y = ball->final_circle().getCentre().second;
    double r = ball->getCircle().getRadius();

    if (x - r < 0 or x + r > arena_size) {
        dx = -dx;
    }
    if (y + dy + r > arena_size) {
         dy = -dy;
    }

    ball->setDeltaVector(dx, dy);
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
        game_data.paddle->set_last_delta(new_x - x);
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

void create_new_ball(DrawingArea& drawing) {
    if (game_data.lives > 0) {
        double x_p = game_data.paddle->getCenter_paddle().first;
        double y_p = game_data.paddle->getCenter_paddle().second;
        double r_p = game_data.paddle->getCircle().getRadius();
        Circle new_ball(x_p, y_p + r_p + new_ball_radius, new_ball_radius);
        if (!new_ball_intersects(new_ball)){
            game_data.balls.push_back(make_unique<Ball>(x_p, y_p + r_p + 
                          new_ball_radius, new_ball_radius, 0.0, new_ball_delta_norm));
            game_data.lives--;
            //update_infos();
            drawing.queue_draw();
        }
    }
}

bool new_ball_intersects(const Circle& new_ball) {
    for (const auto& brick : game_data.bricks) {
        if (brick and intersects(new_ball, brick->getRectangle())) {
            return true;
        }
    }
    for (const auto& ball : game_data.balls) {
        if (ball and intersects(new_ball, ball->getCircle())) {
            return true;
        }
    }
    return false;
}

/*void update_balls(DrawingArea& drawing) {
    if (game_data.balls.empty()) return;
    auto ball = game_data.balls.begin();
    while (ball != game_data.balls.end()) {
        if (*ball) {
            double dx = (*ball)->getDeltaVector().first;
            double dy = (*ball)->getDeltaVector().second;
            double x_b  = (*ball)->getCentre_ball().first;
            double y_b  = (*ball)->getCentre_ball().second;                
            double r  = (*ball)->getCircle().getRadius();
            bool change = true;

            tie (dx, dy) = pre_tests(*ball);

            while ((*ball)->bounce() and change) {
                double dx_o = dx;
                double dy_o = dy;

                // Rebond sur les bricks, le paddle et les autres balls
                tie (dx, dy) = ball_collision(*ball);
                
                // Rebond sur les murs gauche/droite
                if (x_b + dx < r or x_b + dx > arena_size - r) {
                    if ((*ball)->bounce()) {
                        dx = -dx;
                        (*ball)->add_bounce();
                    }
                }
                // Rebond sur le mur du haut
                if (y_b + dy > arena_size - r) {
                    if ((*ball)->bounce()) {
                        dy = -dy;
                        (*ball)->add_bounce();
                    }
                }
                if (dx == dx_o and dy == dy_o) change = false;
                else change = true;
            }
            (*ball)->reset_bounces();
            // Destruction de la ball si sortie par le bas
            if (y_b + dy < 0) {
                ball = game_data.balls.erase(ball);
                //update_infos();
            }
            // Mise à jour du delta et de la position de la ball
            else {
                (*ball)->setDeltaVector(dx, dy);
                ++ball;
            }
        }
    }
    for (const auto& ball : game_data.balls) {
        ball->update_delta();
        ball->update_position();
    }
    new_components();
    drawing.queue_draw();
}*/

/*pair<double, double> pre_tests(const unique_ptr<Ball>& ball) {

    double dx = ball->getDeltaVector().first;
    double dy = ball->getDeltaVector().second;
    double x_b  = ball->getCentre_ball().first;
    double y_b  = ball->getCentre_ball().second;                
    double r  = ball->getCircle().getRadius();
    // Rebond sur le paddle avant le dépacement de la ball    
    if (intersects(Circle(x_b, y_b, r), game_data.paddle->getCircle())) {
        return ball_paddle_collision(dx, dy);
    }
    for (const auto& ball_ : game_data.balls) {
        double dx_ = ball_->getDeltaVector().first;
        double dy_ = ball_->getDeltaVector().second;
        double r_ = ball_->getCircle().getRadius();
        if (ball_ and ball_ != ball) {
            if (intersects(ball->getCircle(), ball_->getCircle())) {
                ball->add_bounce();
                return ball_circle_collision(r, dx, dy,r_, dx_, dy_);
            }
        }
    }
    return {dx, dy};
}*/

void new_components() {
    for (auto& new_ball : game_data.new_balls) {
        game_data.balls.push_back(move(new_ball));
    }
    game_data.new_balls.clear();
    for (auto& new_brick : game_data.new_bricks) {
        game_data.bricks.push_back(move(new_brick));
    }
    game_data.new_bricks.clear();
}

/*pair<double, double> ball_collision(const unique_ptr<Ball>& ball) {
    double dx = ball->getDeltaVector().first;
    double dy = ball->getDeltaVector().second;
    double x_b = ball->getCentre_ball().first;
    double y_b = ball->getCentre_ball().second;
    double r = ball->getCircle().getRadius();
    double dx_o = dx;
    double dy_o = dy;

    if (intersects(Circle(x_b + dx, y_b + dy, r), game_data.paddle->getCircle())) {
        if (ball->bounce()) {
            ball->add_bounce();
            return ball_paddle_collision(dx, dy);
        }
        return {dx_o, dy_o};
    }

    for (const auto& brick : game_data.bricks) {
        if (brick) {
            double x_brick = brick->getRectangle().getCentre().first;
            double y_brick = brick->getRectangle().getCentre().second;
            double w = brick->getRectangle().getWidth();
            if (intersects(Circle(x_b + dx, y_b + dy, r), brick->getRectangle())) {
                update_brick(brick, dx, dy);
                if (ball->bounce()) {
                    ball->add_bounce();
                    return ball_bricks_collision(x_b, y_b, r, dx, dy, x_brick, y_brick,
                                                 w);
                }
                return {dx_o, dy_o};
            }  
        }
    }
    for (const auto& ball_ : game_data.balls) {
        if (ball_ and ball_ != ball) {
            double x_b_c = ball_->getCentre_ball().first;
            double y_b_c = ball_->getCentre_ball().second;
            double r_c = ball_->getCircle().getRadius();
            double dx_c = ball_->getDeltaVector().first;
            double dy_c = ball_->getDeltaVector().second;
            if (intersects(Circle(x_b + dx, y_b + dy, r), Circle(x_b_c + dx_c,
                y_b_c + dy_c, r_c))) {
                if (ball->bounce()) {
                    ball->add_bounce();
                    return ball_circle_collision(r, dx, dy, r_c, dx_c, dy_c);
                }
                return {dx_o, dy_o};
            }
        }
    }
    
    return {dx_o, dy_o};
}*/

void update_brick(const unique_ptr<Brick>& brick, double dx, double dy) {
    double x_ = brick->getRectangle().getCentre().first;
    double y_ = brick->getRectangle().getCentre().second;
    double w_ = brick->getRectangle().getWidth();

    switch (brick->get_destroyed())
    {
    case 0: 
        game_data.bricks.erase(find(game_data.bricks.begin(), game_data.bricks.end(),
                                    brick));
        game_data.score += score_per_hit;
        break;
    
    case 1:
        game_data.score += score_per_hit;
        break;

    case 2:
        game_data.bricks.erase(find(game_data.bricks.begin(), game_data.bricks.end(),
                                    brick));
        game_data.new_balls.push_back(make_unique<Ball>(x_, y_, new_ball_radius, dx, dy));
        game_data.score += score_per_hit;
        break;
    
    case 3:
        new_spltbricks(x_, y_, w_);
        game_data.bricks.erase(find(game_data.bricks.begin(), game_data.bricks.end(),
                                    brick));
        game_data.score += score_per_hit;
        break;
    
    default:
        break;
    }
    
}

void new_spltbricks(double x, double y, double w) {
    double w_n = (w - split_brick_gap) / 2;
    double d = (w + split_brick_gap) / 4;
    game_data.new_bricks.push_back(make_unique<SpltBrick>(x - d, y - d, w_n, w_n));
    game_data.new_bricks.push_back(make_unique<SpltBrick>(x + d, y - d, w_n, w_n));
    game_data.new_bricks.push_back(make_unique<SpltBrick>(x - d, y + d, w_n, w_n));
    game_data.new_bricks.push_back(make_unique<SpltBrick>(x + d, y + d, w_n, w_n));
}

void ball_bricks_collision(const unique_ptr<Ball>& ball, const unique_ptr<Brick>& brick) {        
    // Coordonnées centre balle
    double x_b = ball->getCentre_ball().first;
    double y_b = ball->getCentre_ball().second;
    // Rayon balle
    double r = ball->getCircle().getRadius();
    // Coordonnées centre rectangle
    double x_brick = brick->getRectangle().getCentre().first;
    double y_brick = brick->getRectangle().getCentre().second;
    // Largeur rectangle
    double w = brick->getRectangle().getWidth();
    double half_w = w/2;
    // Coordonnées vecteur deplacement balle
    double dx = ball->getDeltaVector().first;
    double dy = ball->getDeltaVector().second;
    update_brick(brick, dx, dy);

    double diff_x = x_brick - x_b;
    double diff_y = y_brick - y_b;

    double diff_born_x = check_inclusion(diff_x, half_w);
    double diff_born_y = check_inclusion(diff_y, half_w);

    double d_nom_x = diff_x - diff_born_x;
    double d_nom_y = diff_y - diff_born_y;

    double norm = sqrt(d_nom_x*d_nom_x + d_nom_y*d_nom_y);

    if (norm > 0) {
        double nx = d_nom_x / norm;
        double ny = d_nom_y / norm;
        
        double dot = dx*nx + dy*ny;
        dx = dx - 2*dot*nx;
        dy = dy - 2*dot*ny;
    }
    ball->setDeltaVector(limit_delta(dx, dy).first, limit_delta(dx, dy).second);
}  

double check_inclusion(double h, double diff){
    
    if (diff < -h) return -h; 
    if (diff > h) return h;
    else return diff;
}

void ball_paddle_collision(const unique_ptr<Ball>& ball) {
    double dx = ball->getDeltaVector().first;
    double dy = ball->getDeltaVector().second;
    double dx_p = game_data.paddle->getLast_delta();

    // Position balle et paddle
    double x_i = ball->getCentre_ball().first;
    double y_i = ball->getCentre_ball().second;
    double x_p = game_data.paddle->getCenter_paddle().first;
    double y_p = game_data.paddle->getCenter_paddle().second;

    // Normale balle → centre raquette
    double dist = sqrt(pow(x_p - x_i, 2) + pow(y_p - y_i, 2));
    double nx = (x_p - x_i) / dist;
    double ny = (y_p - y_i) / dist;

    // Projection des vitesses sur la normale
    double vn_ball   = dx*nx + dy*ny;
    double vn_paddle = dx_p*nx; // dy_paddle = 0

    // Rayon raquette → infini donc coeff = 2
    double impulsion = (-vn_ball + vn_paddle) * 2.0;

    double dx_new = dx + impulsion * nx;
    double dy_new = dy + impulsion * ny;

    auto [dx_lim, dy_lim] = limit_delta(dx_new, dy_new);
    ball->setDeltaVector(dx_lim, dy_lim);
}

void ball_circle_collision(const unique_ptr<Ball>& ball, const unique_ptr<Ball>& ball_) {
    // double r = ball->getCircle().getRadius();
    // double dx = ball->getDeltaVector().first;
    // double dy = ball->getDeltaVector().second;
    // double r_c = ball_->next_circle().getRadius();
    // double dx_c = ball_->get_copy_deltaVector().first;
    // double dy_c = ball_->get_copy_deltaVector().second;

    // double coeff = 2*pow(r_c,2)/(pow(r,2) + pow(r_c,2));

    // double dx_ = dx + coeff*(dx_c - dx);
    // double dy_ = dy + coeff*(dy_c - dy);

    // tie (dx_, dy_) = limit_delta(dx_, dy_);

    // ball->setDeltaVector(dx_, dy_);

    double r_i = ball->getCircle().getRadius();
    double dx_i = ball->getDeltaVector().first;
    double dy_i = ball->getDeltaVector().second;
    double x_i = ball->getCentre_ball().first;
    double y_i = ball->getCentre_ball().second;

    double r_j = ball_->getCircle().getRadius();
    double dx_j = ball_->getDeltaVector().first;
    double dy_j = ball_->getDeltaVector().second;
    double x_j = ball_->getCentre_ball().first;
    double y_j = ball_->getCentre_ball().second;

    //Distance normale entre les deux centres (checker la proximité des balles)
    double x_norm = (x_j - x_i) / sqrt(pow(x_i - x_j, 2) + pow(y_i - y_j, 2));
    double y_norm = (y_j - y_i) / sqrt(pow(x_i - x_j, 2) + pow(y_i - y_j, 2));

    //Projection des vitesses sur la normale
    double v_norm_i = dx_i*x_norm + dy_i*y_norm; // v de ball_i selon n
    double v_norm_j = dx_j*x_norm + dy_j*y_norm; // v de ball_j selon n
    

    double impulsion_i = (-v_norm_i + v_norm_j)*(2*pow(r_j,2) / (pow(r_i,2) + pow(r_j,2)));
    double impulsion_j = (-v_norm_j + v_norm_i)*(2*pow(r_i,2) / (pow(r_i,2) + pow(r_j,2)));


    // Implémentation du nouveau delta
    double dx_new_i = dx_i + impulsion_i*x_norm;
    double dy_new_i = dy_i + impulsion_i*y_norm;
    
    double dx_new_j = dx_j + impulsion_j*x_norm;
    double dy_new_j = dy_j + impulsion_j*y_norm;

    auto[dx_lim_i, dy_lim_i] = limit_delta(dx_new_i, dy_new_i);
    auto[dx_lim_j, dy_lim_j] = limit_delta(dx_new_j, dy_new_j);

    if (ball->bounce()) {
        ball->setDeltaVector(dx_lim_i, dy_lim_i);
        ball->add_bounce();
    }

    if (ball_->bounce()) {
        ball_->setDeltaVector(dx_lim_j, dy_lim_j);
        ball_->add_bounce();
    }
}

bool game_ended() {
    if (game_data.lives == 0 and game_data.balls.empty() 
             && !game_data.game_ended) {
        cout << message::lost() << endl;
        game_data.game_ended = true;
    }
    else if (game_data.bricks.empty() && !game_data.game_ended) {
        game_data.score += score_per_life*game_data.lives;
        cout << message::won() << endl;
        game_data.game_ended = true;
    }
    return game_data.game_ended;
}