EXEC = kuribrawl

CFLAGS = `sdl2-config --cflags` -g -Iinclude -std=c++17
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

vpath %.h include
vpath %.cpp src

_OBJS = Animation.o app.o Champion.o CurrentAnimation.o Fighter.o Game.o GameData.o KBException.o load.o main.o sdlHelper.o
OBJS = $(patsubst %,bin/%,$(_OBJS))

all: $(EXEC)

bin/%.o: %.cpp 
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/*

$(EXEC): $(OBJS)
	g++ $^ -o $@ $(LDFLAGS) 