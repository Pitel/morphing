CXXFLAGS=-O2 -pipe -pedantic-errors -Wall -Wextra -march=native `pkg-config --cflags opencv`
LDFLAGS=-lcv -lhighgui

.PHONY: clean

all: morphing

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $<

morphing: cvmorph.o

clean:
	$(RM) morphing *.o
