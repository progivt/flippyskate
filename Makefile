OBJS = \
    main.o \
    game.o \
    entity.o \


REBUILDABLES = $(OBJS) flappy

vpath %.cpp src
vpath %.hpp include

all: flappy
	./flappy

flappy: $(OBJS)
	clang++ -F /Library/Frameworks -framework SDL2 -framework SDL2_image -g -o flappy $^

%.o: %.cpp
	clang++ -g -std=c++14 -I./include -F /Library/Frameworks -o $@ -c $<

main.o: game.hpp entity.hpp
game.o: game.hpp entity.hpp
entity.o: entity.hpp

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

