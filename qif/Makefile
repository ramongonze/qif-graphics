CFLAGS=-std=c++11 -O4 -Wall -Wextra -Werror -pedantic

all: qif.o distribution.o channel.o gain.o hyper.o vulnerability.o entropy.o leakage.o binary

qif.o:
	g++ -c ./src/qif.cpp $(CFLAGS)

distribution.o:
	g++ -c ./src/distribution.cpp $(CFLAGS)

channel.o:
	g++ -c ./src/channel.cpp $(CFLAGS)

gain.o:
	g++ -c ./src/gain.cpp $(CFLAGS)

hyper.o:
	g++ -c ./src/hyper.cpp $(CFLAGS)

vulnerability.o:
	g++ -c ./src/vulnerability.cpp $(CFLAGS)

entropy.o:
	g++ -c ./src/entropy.cpp $(CFLAGS)

leakage.o:
	g++ -c ./src/leakage.cpp $(CFLAGS)

binary:
	ar rcs qif.a distribution.o channel.o gain.o hyper.o vulnerability.o entropy.o leakage.o
	rm -r -f bin
	mkdir bin
	mv *.o ./bin/