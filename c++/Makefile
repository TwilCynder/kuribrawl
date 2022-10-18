EXEC = kuribrawl

ifdef RELEASE
	CFLAGS = `sdl2-config --cflags` -Wall -Iinclude -std=gnu++2a -O3
else
	CFLAGS = `sdl2-config --cflags` -Wall -g -Iinclude -std=gnu++2a -DDEBUG=1
endif 


LDFLAGS = `sdl2-config --static-libs` -static-libgcc -static-libstdc++ -lSDL2_image -lm
SRC= $(wildcard src/*.cpp src/**/*.cpp)
_OBJS= $(SRC:.cpp=.o)

vpath %.h include
vpath %.cpp src

OBJS = $(patsubst src/%,bin/%,$(_OBJS))
OBJDIRS:=$(dir $(OBJECTS))

NB_FILES = $(words $(SRC))
CURRENT_NB = 0

all: start $(EXEC)

release: all
	cp kuribrawl.exe ../release/files/kuribrawl.exe
	cp ../res/data.twl ../release/files/data.twl

launch: all
	./kuribrawl.exe

clean: clear all

_create_doc_dir:
	mkdir -p doc

doc: _create_doc_dir
	doxygen

docG: _create_doc_dir
	doxygen DoxyfileGraph

docGF: _create_doc_dir
	doxygen DoxyfileGraphFull

start: 
	@if [ -z "$(NOCLEAR)" ]; then\
		clear;\
	fi
	@printf "\n\033[1;33mStarted compilation\n\033[0m"
	@mkdir -p bin

bin/%.o: %.cpp #$(SRC)
	$(eval CURRENT_NB=$(shell echo $$(($(CURRENT_NB)+1))))
	@printf "\033[1;33m[$(CURRENT_NB)/$(NB_FILES)]\033[0m "
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -c $< -o $@

clear:
	touch bin/d
	-rm -r bin/*
	-rm *.exe

$(EXEC): $(OBJS)
	@printf "\033[1;33m[Linking]\033[0m "
	g++ $^ -o $@ $(LDFLAGS) 
	@printf "\033[0;32mCompilation finished successfully !\n\033[0m"
