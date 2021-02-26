EXEC = kuribrawl

CFLAGS = `sdl2-config --cflags` -g -Iinclude
LDFLAGS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lm

vpath %.h include
vpath %.cpp src

_OBJS = app.o main.o sdlHelper.o Animation.o KBException.o
OBJS = $(patsubst %,bin/%,$(_OBJS))

all: $(EXEC)

bin/%.o: %.cpp 
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -r bin/*

$(EXEC): $(OBJS)
	g++ $^ -o $@ $(LDFLAGS) 