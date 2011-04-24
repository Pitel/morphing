CXXFLAGS=-O2 -pipe -pedantic-errors -Wall -Wextra -march=native `pkg-config --cflags opencv gtk+-2.0`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0`

.PHONY: clean

all: morphing

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $<

morphing: cvmorph.o

gui: cvmorph.o morphing_gui.cpp
	$(CXX) $(CXXFLAGS) morphing_gui.cpp -o morphing_gui

clean:
	$(RM) morphing *.o
