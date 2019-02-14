all: clean main

clean:
	rm -f *.o
	rm -f *.out

main: shader.o graphics.o
	g++ main.cpp bin/shader.o bin/graphics.o qif/qif.a -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lfreetype -I/usr/local/include/freetype2 /usr/local/src/glad.o /usr/lib/x86_64-linux-gnu/libdl.so

shader.o:
	g++ -c ./src/shader.cpp
	mv shader.o ./bin

graphics.o:
	g++ -c ./src/graphics.cpp
	mv graphics.o ./bin