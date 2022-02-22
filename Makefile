all:
	clang++ src/*.cpp  -I./include -F /Library/Frameworks -framework SDL2 -framework SDL2_image -o flappy
