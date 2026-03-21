#include <string>
#include <iostream>
#include <sstream>
#include "read.h"
#include "objects.h"

using namespace std;

// Fonction principale
int main(int argc, char *argv[]) {

    if(read(argv[1])) {
        cout << "Lecture reussie" << endl;
    } else {
        cout << "Erreur lors de la lecture du fichier." << endl;
    }
    /*
    cout << "Score: " << score << endl;
    cout << "Lives: " << lives << endl;
    cout << "Paddle: " << paddle << endl;
    cout << "Number of Bricks: " << nb_brick << endl;
    cout << "Number of Balls: " << nb_ball << endl;
    */
}
 