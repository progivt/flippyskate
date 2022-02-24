OBJS = \
  build/main.o \
  build/engine.o \
  build/entity.o \
  build/gameplay.o

REBUILDABLES = $(OBJS) flappy

vpath %.cpp src
vpath %.hpp include
vpath %.o build

all: flappy
	./build/flappy

flappy: $(OBJS)
	clang++ -F /Library/Frameworks -framework SDL2 -framework SDL2_image -g -o build/flappy $^

build/%.o: %.cpp
	clang++ -g -std=c++17 -I./include -F /Library/Frameworks -o $@ -c $<

build/main.o: engine.o
build/engine.o: entity.o gameplay.o engine.hpp
build/entity.o: entity.hpp
build/gameplay.o: gameplay.hpp

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

