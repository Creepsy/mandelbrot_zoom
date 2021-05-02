bDir = build/
srcDir = src/
libDir = lib/

args = -g

all: $(bDir)mandelbrot_zoom.o $(bDir)glad.o $(bDir)shaders.o
	g++ $(bDir)mandelbrot_zoom.o -o mandelbrot_zoom $(bDir)glad.o $(bDir)shaders.o -lglfw -ldl $(args)

$(bDir)mandelbrot_zoom.o: $(srcDir)mandelbrot_zoom.cpp
	g++ -c $(srcDir)mandelbrot_zoom.cpp -o $(bDir)mandelbrot_zoom.o $(args)

$(bDir)shaders.o: $(srcDir)shaders.cpp
	g++ -c $(srcDir)shaders.cpp -o $(bDir)shaders.o $(args)

$(bDir)glad.o: $(libDir)glad/glad.h $(libDir)glad/glad.c
	g++ -c $(libDir)glad/glad.c -o $(bDir)glad.o $(args)