OBJS = \
    main.o \
    game.o \
    glad.o


REBUILDABLES = $(OBJS) flappy

all: flappy
	./flappy

flappy: $(OBJS)
	clang++ -F /Library/Frameworks -framework SDL2 -framework SDL2_image -g -o flappy $^

%.o: src/%.cpp
	clang++ -g -std=c++14 -I./include -F /Library/Frameworks -o $@ -c $<

# main.o : main.h

clean: 
	rm -f $(REBUILDABLES)
	echo Clean done

