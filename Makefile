CXXFLAGS=-O2 -pipe -pedantic -Wall -Wextra -march=native `pkg-config --cflags opencv gtk+-2.0 cairo`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0 cairo`

.PHONY: clean

all: morphing

#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $<

morphing: gui_setgridwnd.o cvmorph.o

morphing-test: cvmorph.o

clean:
	$(RM) morphing-test morphing *.o
