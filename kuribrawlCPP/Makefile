EXEC = kuribrawl

CFLAGS = `sdl2-config --cflags` -g -Iinclude -std=c++17
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

SRC= $(wildcard src/*.cpp)
_OBJS= $(SRC:.cpp=.o)


vpath %.h include
vpath %.cpp src

#_OBJS = Animation.o app.o Champion.o CurrentAnimation.o Fighter.o Game.o GameData.o KBException.o load.o main.o sdlHelper.o Test.o
OBJS = $(patsubst src/%,bin/%,$(_OBJS))

all: clear $(EXEC)

clear: 
	clear
	@printf "\n\033[1;33mStarted compilation\n\033[0m"

bin/%.o: %.cpp 
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/*

$(EXEC): $(OBJS)
	g++ $^ -o $@ $(LDFLAGS) 
	@printf "\033[0;32mCompilation finished successfully !"
