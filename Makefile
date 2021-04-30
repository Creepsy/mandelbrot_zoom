bDir = build/
srcDir = src/
libDir = lib/

all: $(bDir)mandelbrot_zoom.o
	g++ $(bDir)mandelbrot_zoom.o -o mandelbrot_zoom

$(bDir)mandelbrot_zoom.o: $(srcDir)mandelbrot_zoom.cpp
	g++ -c $(srcDir)mandelbrot_zoom.cpp -o $(bDir)mandelbrot_zoom.o

