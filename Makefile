EXE = ice-maze
OBJS = ice-maze.o window.o camera.o level.o player.o unit.o key.o gate.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror -std=c++11
LINKER = g++
LINKER_OPTS = -I/usr/include -L/usr/lib -lGL -lGLU -lSDL2

all: $(EXE)

$(EXE): $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) -o $(EXE)

ice-maze.o: ice-maze.cpp window.h player.h level.h gate.h key.h
	$(COMPILER) $(COMPILER_OPTS) ice-maze.cpp

window.o: window.cpp window.h
	$(COMPILER) $(COMPILER_OPTS) window.cpp

camera.o: camera.cpp camera.h
	$(COMPILER) $(COMPILER_OPTS) camera.cpp

level.o: level.cpp level.h gate.h key.h unit.h
	$(COMPILER) $(COMPILER_OPTS) level.cpp

player.o: player.cpp player.h
	$(COMPILER) $(COMPILER_OPTS) player.cpp

unit.o: unit.cpp unit.h
	$(COMPILER) $(COMPILER_OPTS) unit.cpp

gate.o: gate.cpp gate.h
	$(COMPILER) $(COMPILER_OPTS) gate.cpp

key.o: key.cpp key.h
	$(COMPILER) $(COMPILER_OPTS) key.cpp

clean:
	rm -r *.o $(EXE)
