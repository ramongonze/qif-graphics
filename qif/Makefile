# Compile for desktop

CFLAGS_DESKTOP=-std=c++11 -O4 -Wall -Wextra -Werror -pedantic

desktop: qif.o distribution.o channel.o gain.o hyper.o vulnerability.o entropy.o leakage.o binary

qif.o:
	g++ -c ./src/qif.cpp $(CFLAGS_DESKTOP)

distribution.o:
	g++ -c ./src/distribution.cpp $(CFLAGS_DESKTOP)

channel.o:
	g++ -c ./src/channel.cpp $(CFLAGS_DESKTOP)

gain.o:
	g++ -c ./src/gain.cpp $(CFLAGS_DESKTOP)

hyper.o:
	g++ -c ./src/hyper.cpp $(CFLAGS_DESKTOP)

vulnerability.o:
	g++ -c ./src/vulnerability.cpp $(CFLAGS_DESKTOP)

entropy.o:
	g++ -c ./src/entropy.cpp $(CFLAGS_DESKTOP)

leakage.o:
	g++ -c ./src/leakage.cpp $(CFLAGS_DESKTOP)

binary:
	ar rcs qif.a distribution.o channel.o gain.o hyper.o vulnerability.o entropy.o leakage.o
	rm -r -f bin
	mkdir bin
	mv *.o ./bin/

# Compile for web
CFLAGS_WEB=-std=c++11 -O3 -Wall -Wextra -Werror -pedantic

web: qifd.bc distributiond.bc channeld.bc gaind.bc hyperd.bc vulnerabilityd.bc entropyd.bc leakaged.bc binary

qifd.bc:
	em++ -c ./src/qif.cpp $(CFLAGS_WEB) -o qif.bc

distributiond.bc:
	em++ -c ./src/distribution.cpp $(CFLAGS_WEB) -o distribution.bc

channeld.bc:
	em++ -c ./src/channel.cpp $(CFLAGS_WEB) -o channel.bc

gaind.bc:
	em++ -c ./src/gain.cpp $(CFLAGS_WEB) -o gain.bc

hyperd.bc:
	em++ -c ./src/hyper.cpp $(CFLAGS_WEB) -o hyper.bc

vulnerabilityd.bc:
	em++ -c ./src/vulnerability.cpp $(CFLAGS_WEB) -o vulnerability.bc

entropyd.bc:
	em++ -c ./src/entropy.cpp $(CFLAGS_WEB) -o entropy.bc

leakaged.bc:
	em++ -c ./src/leakage.cpp $(CFLAGS_WEB) -o leakage.bc

binary:
	emar rcs qif.a distribution.bc channel.bc gain.bc hyper.bc vulnerability.bc entropy.bc leakage.bc
	rm -r -f bin
	mkdir bin
	mv *.bc ./bin/

clean:
	rm -rf bin/*
	rm -rf qif.a