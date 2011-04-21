CXXFLAGS=-O2 -pipe -pedantic-errors -g -Wall -Wextra -march=native `pkg-config --cflags opencv`
LDFLAGS=-lcv -lhighgui

.PHONY: clean

all: zadani

zadani:

clean:
	$(RM) zadani
