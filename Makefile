# # Definitions de macros

# CXX     = g++
# CXXFLAGS = -g -Wall -Wextra -std=c++17
# CXXFILES = project.cc game.cc message.cc bricks.cc ball.cc paddle.cc tools.cc
# OFILES = $(CXXFILES:.cc=.o)

# # Definition de la premiere regle

# project: $(OFILES)
# 	$(CXX) $(CXXFLAGS) $(OFILES) -o project

# %.o: %.cc
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Definitions de cibles particulieres

# depend:
# 	@echo " *** MISE A JOUR DES DEPENDANCES ***"
# 	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
# 	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
# 	  egrep -v "/usr/include" \
# 	 ) > Makefile.new
# 	@mv Makefile.new Makefile

# clean:
# 	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
# 	@/bin/rm -f *.o *.x *.cc~ *.h~ project

# #
# # -- Regles de dependances generees automatiquement
# #
# # DO NOT DELETE THIS LINE
# project.o: project.cc game.h message.h constants.h tools.h bricks.h \
#   ball.h paddle.h
# game.o: game.cc message.h constants.h tools.h game.h bricks.h ball.h \
#   paddle.h
# message.o: message.cc message.h
# bricks.o: bricks.cc bricks.h tools.h ball.h constants.h
# ball.o: ball.cc ball.h tools.h constants.h
# paddle.o: paddle.cc paddle.h tools.h constants.h
# tools.o: tools.cc tools.h constants.h


OUT      := project
CXX      := g++
CXXFLAGS = -std=c++17 -I$(INCLUDE_DIR) -Wall -g $(shell pkg-config --cflags gtkmm-4.0)
PKGS     := gtkmm-4.0
LINKING  := $(shell pkg-config --cflags $(PKGS))
LDLIBS   := $(shell pkg-config --libs $(PKGS))

BUILD_DIR := build

CXXFILES := project.cc game.cc message.cc bricks.cc ball.cc paddle.cc tools.cc graphic.cc gui.cc
OFILES   := $(addprefix $(BUILD_DIR)/, $(CXXFILES:.cc=.o))

.PHONY: all clean tests

all: $(OUT)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@

$(OUT): $(OFILES)
	@$(CXX) $(CXXFLAGS) $(LINKING) $^ -o $@ $(LDLIBS)

clean:
	@echo "Cleaning project..."
	@rm -rf $(BUILD_DIR) $(OUT)

tests: $(OUT)
	@for test in $$(ls tests); do \
		echo "Running $$test..."; \
		./$(OUT) tests/$$test; \
	done
