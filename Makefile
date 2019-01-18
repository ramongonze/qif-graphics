all: clean main

clean:
	rm -f *.o
	rm -f *.out

main: graphics.o
	g++ main.cpp graphics.o qif/qif.a -lglfw -lrt -lm -ldl -lGLEW -lGL

graphics.o:
	g++ -c graphics.cpp