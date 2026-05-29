SRC_FILES=$(wildcard src/*.c)

bin/app: $(SRC_FILES) glad/src/gl.c
	gcc -Iglad/include -Iinclude -lSDL3 -o $@ $^
