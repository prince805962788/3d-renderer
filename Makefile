build:
	gcc -Wall -std=c99 ./src/*.c -o renderer $(shell sdl2-config --cflags --libs)

run:
	./renderer

clean:
	rm renderer
