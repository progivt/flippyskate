OBJS = \
  build/game.o \
  build/scene.o \
  build/entity.o \
  build/engine.o

REBUILDABLES = $(OBJS) flappy

vpath %.cpp src
vpath %.hpp include
vpath %.o build

all: flappy
	./build/flappy

flappy: $(OBJS)
	clang++ -F /Library/Frameworks \
	    -framework SDL2 -framework SDL2_image -framework SDL2_ttf -g \
	    -o build/flappy $^

build/%.o: %.cpp
	clang++ -g -std=c++17 -I./include -F /Library/Frameworks -o $@ -c $<

build/game.o: game.hpp scene.o engine.o entity.o
build/engine.o: engine.hpp entity.o 
build/scene.o: scene.hpp entity.o 
build/entity.o: entity.hpp

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

