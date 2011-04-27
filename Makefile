CXXFLAGS=-O2 -pipe -pedantic -Wall -Wextra -march=native `pkg-config --cflags opencv gtk+-2.0 cairo`
LDFLAGS=`pkg-config --libs opencv gtk+-2.0 cairo`

.PHONY: clean doc

all: morphing

#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $<

morphing: gui_setgridwnd.o cvmorph.o

morphing-test: cvmorph.o

clean:
	$(RM) morphing-test morphing *.o xlogin00.zip README.html

doc:
	echo '<!DOCTYPE html><html><head><meta charset="utf-8"><title>Morphing</title></head><body>' > README.html && perl Markdown/Markdown.pl README.md >> README.html && echo '</body></html>' >> README.html

pack: doc
	zip -9 xlogin00.zip *.cpp *.hpp Makefile README.html Lenna.png bush.png
