# Makefile pour le projet COM-112
# Utilise g++ pour compiler

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = game
SOURCES = game.cc read.cc message.cc objects.cc tools.cc
HEADERS = read.h message.h constants.h objects.h tools.h

# Règle par défaut : compile le programme
all: $(TARGET)

# Compilation du programme
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET)

# Recompilation complète
rebuild: clean all

# Test avec t00.txt (ajuste si nécessaire)
test: $(TARGET)
	./$(TARGET) tests/t00.txt

.PHONY: all clean rebuild test