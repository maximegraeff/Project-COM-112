# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++17
CXXFILES = project.cc game.cc message.cc objects.cc tools.cc
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle

project: $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o project

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) > Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ project

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
