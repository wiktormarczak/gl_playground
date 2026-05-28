bin/app: src/main.c glad/src/gl.c
	g++ -Iglad/include -lSDL3 -o $@ $^
