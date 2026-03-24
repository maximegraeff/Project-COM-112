// game.cc : fichier principal du projet, contenant la fonction main qui lance le jeu.
//
//
// Version 1.4 du 24.03.2025
//
#include <string>
#include <iostream>
#include <sstream>
#include "read.h"
#include "objects.h"

using namespace std;

// Fonction principale : ./game teats/t00.txt
int main(int argc, char *argv[]) {
    
    // Lecture des données du fichier et initialisation des objets
    read(argv[1]);
}