OBJS = \
  build/game.o \
  build/engine.o \
  build/startscreen.o \
  build/gamescreen.o \
  build/scene.o \
  build/entity.o \
  build/util.o

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

build/game.o: game.hpp gamescreen.o scene.o engine.o entity.o util.o
build/engine.o: engine.hpp entity.o util.o
build/startscreen.o: startscreen.hpp scene.o entity.o util.o
build/gamescreen.o: gamescreen.hpp scene.o entity.o util.o
build/scene.o: scene.hpp entity.o util.o
build/entity.o: entity.hpp util.o
build/util.o: util.hpp

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

