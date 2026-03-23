#include <string>
#include <iostream>
#include <sstream>
#include "read.h"
#include "objects.h"

using namespace std;

// Fonction principale : g++ -g -Wall -std=c++11 game.cc read.cc message.cc objects.cc tools.cc -o game && ./game.exe < tests\t00.txt
int main(int argc, char *argv[]) {
    
    read(argv[1]);

    /*
    cout << "Score: " << score << endl;
    cout << "Lives: " << lives << endl;
    cout << "Paddle: " << paddle << endl;
    cout << "Number of Bricks: " << nb_brick << endl;
    cout << "Number of Balls: " << nb_ball << endl;
    */
}