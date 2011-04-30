CXX=g++-4.5
CXXFLAGS=-O2 -pipe -pedantic -Wall -Wextra -march=native -std=gnu++0x `pkg-config --cflags opencv gtk+-2.0 cairo`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0 cairo`

.PHONY: clean doc

all: morphing

morphing: gui_setgridwnd.o cvmorph.o globals.o

morphing-test: cvmorph.o

clean:
	$(RM) morphing-test morphing *.o xlogin00.zip README.html

doc:
	echo '<!DOCTYPE html><html><head><meta charset="utf-8"><title>Morphing</title></head><body>' > README.html && markdown README.md >> README.html && echo '</body></html>' >> README.html

pack: doc
	zip -9 xlogin00.zip *.cpp *.hpp Makefile README.html lenna.jpg bush.jpg cibo00_Algebraic_Mosaic.svg
