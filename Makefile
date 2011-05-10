CXX=g++-4.5
CXXFLAGS=-O2 -pipe -pedantic -Wall -Wextra -march=native -std=gnu++0x `pkg-config --cflags opencv gtk+-2.0 cairo`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0 cairo`

.PHONY: clean pack

all: morphing

morphing: gui_setgridwnd.o cvmorph.o globals.o

morphing-test: cvmorph.o

clean:
	$(RM) morphing-test morphing *.o xlogin00.zip

pack:
	zip -9 xlogin00.zip *.cpp *.hpp Makefile README.pdf *.jpg cibo00_Algebraic_Mosaic.svg
