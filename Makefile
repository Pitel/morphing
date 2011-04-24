CXXFLAGS=-O2 -pipe -pedantic -Wall -Wextra -march=native `pkg-config --cflags opencv gtk+-2.0 cairo`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0 cairo`

.PHONY: clean

all: morphing
OBJS=gui_setgridwnd.o cvmorph.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $<

morphing: cvmorph.o

#cvmorph.o: cvmorph.cpp cvmorph.hpp
#gui_setgridwnd.o: gui_setgridwnd.cpp gui_setgridwnd.hpp globals.hpp

gui: gui_setgridwnd.cpp gui_setgridwnd.hpp globals.hpp cvmorph.cpp cvmorph.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) gui_setgridwnd.cpp gui_setgridwnd.hpp globals.hpp cvmorph.cpp cvmorph.hpp morphing_gui.cpp -o morphing_gui

clean:
	$(RM) morphing morphing_gui *.o
