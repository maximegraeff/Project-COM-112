#include <string>
#include <iostream>
#include <sstream>
#include "read.h"
#include "objects.h"

using namespace std;

// Fonction principale : g++ -std=c++17 game.cc read.cc message.cc objects.cc tools.cc -o game.exe && game.exe tests\t00.txt
int main(int argc, char *argv[]) {
    
    read(argv[1]);

}