OBJS = \
    main.o \
    engine.o \
    entity.o \
    gameplay.o


REBUILDABLES = $(OBJS) flappy

vpath %.cpp src
vpath %.hpp include

all: flappy
	./flappy

flappy: $(OBJS)
	clang++ -F /Library/Frameworks -framework SDL2 -framework SDL2_image -g -o flappy $^

%.o: %.cpp
	clang++ -g -std=c++14 -I./include -F /Library/Frameworks -o $@ -c $<

main.o: engine.o
engine.o: entity.o gameplay.o
entity.o: entity.hpp
gameplay.o: gameplay.hpp

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

