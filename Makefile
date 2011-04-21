CXXFLAGS=-O2 -pipe -pedantic-errors -Wall -Wextra -march=native -fomit-frame-pointer `pkg-config --cflags opencv`
LDFLAGS=-lcv -lhighgui

.PHONY: clean

all: zadani

zadani:

clean:
	$(RM) zadani
