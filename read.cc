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

using namespace std;

// Enumération pour les codes d'erreur
enum ErrorCode { OPENING, READING };

enum ObjectType { SCORE, LIVES, PADDLE, BRICK, CO_BRICK, BALL, CO_BALL };

static unsigned object(SCORE);
static int score(0);
static int lives(0);
static string paddle("");
static int nb_brick(0);
static int count_brick(0);
static int nb_ball(0);

// Prototypes des fonctions
void error(ErrorCode code);
bool read(string filename);
void use_data(string line);

// Fonction principale
int main(int argc, char *argv[]) {
    if(read(argv[1])) {
        cout << "Lecture reussie" << endl;
    } else {
        cout << "Erreur lors de la lecture du fichier." << endl;
    }
    cout << "Score: " << score << endl;
    cout << "Lives: " << lives << endl;
    cout << "Paddle: " << paddle << endl;
    cout << "Number of Bricks: " << nb_brick << endl;
    cout << "Number of Balls: " << nb_ball << endl;
}

// Fonction qui lit le fichier et utilise les données
bool read(string filename)
{
    ifstream file(filename);

    if (!file.fail()) 
    {
        string line;

        while (getline(file >> ws, line)) {

            if(line[0]=='#') continue;

            use_data(line);
        
        }

        return true;
    }

    error(OPENING);
    return false;
}

// Fonction qui utilise les données lues pour vérifier leur validité et les stocker dans les variables globales
void use_data(string line)
{
    switch (object)
    {
        case SCORE: 
        {
            if (stoi(line) >= 0) score = stoi(line);
            else cout << message::invalid_score(stoi(line)) << endl;   // Vérification de la validité du score
            object = LIVES;
            break;
        }

        case LIVES:
        {
            if (stoi(line) >= 0) lives = stoi(line);
            else cout << message::invalid_lives(stoi(line)) << endl;    // Vérification de la validité du nombre de vies
            object = PADDLE;
            break;
        }

        case PADDLE:
        {
            istringstream passor(line);
            int x, y, radius;
            passor >> x >> y >> radius;
            double width = sqrt(pow(radius, 2) - pow(y, 2));
            if (y + radius <= 0 or y > 0  or x - width < 0 or x + width > arena_size) cout << message::paddle_outside(x, y) << endl;    // Vérification de la position du paddle
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
            count_brick++;
            if (count_brick == nb_brick) object = BALL;
            istringstream passor(line);
            int type, x, y, size, hit_points;
            passor >> type >> x >> y >> size >> hit_points;
            if (size < brick_size_min) cout << message::invalid_brick_size(size) << endl;           // Vérification de la validité de la taille de la brick
            else if (type < 0 or type > 2) cout << message::invalid_brick_type(type) << endl;       // Vérification de la validité du type de la brick
            else if (hit_points < 0 or hit_points > 7) cout << message::invalid_hit_points(hit_points) << endl;                                      // Vérification de la validité du hit points de la brick
            else if (x - size < 0 or x + size > arena_size or y - size < 0 or y + size > arena_size) cout << message::brick_outside(x, y) << endl;   // Vérification de la position du brick
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
            int x, y, radius, delta_x, delta_y;
            passor >> x >> y >> radius >> delta_x >> delta_y;
            if (x - radius < 0 or x + radius > arena_size or y < 0 or y + radius > arena_size) cout << message::ball_outside(x, y) << endl;   // Vérification de la position du ball
            if (sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > delta_norm_max) cout << message::invalid_delta(delta_x, delta_y) << endl;           // Vérification de la validité du delta
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
