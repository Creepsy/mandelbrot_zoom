bDir = build/
srcDir = src/
libDir = lib/

all: $(bDir)mandelbrot_zoom.o
	g++ $(bDir)mandelbrot_zoom.o -o mandelbrot_zoom

$(bDir)mandelbrot_zoom.o: $(srcDir)mandelbrot_zoom.cpp
	g++ -c $(srcDir)mandelbrot_zoom.cpp -o $(bDir)mandelbrot_zoom.o


$(bDir)glad.o: $(libDir)glad/glad.h $(libDir)glad/glad.c
	g++ -c $(libDir)glad/glad.c -o $(bDir)glad.o