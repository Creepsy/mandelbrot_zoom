bDir = build/
srcDir = src/
libDir = lib/

all: $(bDir)mandelbrot_zoom.o $(bDir)glad.o $(bDir)shaders.o
	g++ $(bDir)mandelbrot_zoom.o -o mandelbrot_zoom $(bDir)glad.o $(bDir)shaders.o -lglfw -ldl

$(bDir)mandelbrot_zoom.o: $(srcDir)mandelbrot_zoom.cpp
	g++ -c $(srcDir)mandelbrot_zoom.cpp -o $(bDir)mandelbrot_zoom.o

$(bDir)shaders.o: $(srcDir)shaders.cpp
	g++ -c $(srcDir)shaders.cpp -o $(bDir)shaders.o

$(bDir)glad.o: $(libDir)glad/glad.h $(libDir)glad/glad.c
	g++ -c $(libDir)glad/glad.c -o $(bDir)glad.o