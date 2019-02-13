all: clean test

clean:
	rm -f *.o
	rm -f *.out

test: shader.o graphics.o
	g++ test.cpp bin/shader.o bin/graphics.o qif/qif.a -lglfw -lGL -lX11 -lpthread -lXrandr -ldl /usr/local/src/glad.o /usr/lib/x86_64-linux-gnu/libdl.so

shader.o:
	g++ -c ./src/shader.cpp
	mv shader.o ./bin

graphics.o:
	g++ -c ./src/graphics.cpp
	mv graphics.o ./bin