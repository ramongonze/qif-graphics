# Flags to compile raylib
# Write in the variable below with the complete raylib directory
RAYLIB_DIR=/home/ramon/Desktop/qif-graphics/

CFLAGS= -D_DEFAULT_SOURCE \
-DPLATFORM_DESKTOP \
-Wno-missing-braces \
-I/usr/local/include \
-isystem. \
-isystem$(RAYLIB_DIR)raylib/src \
-isystem$(RAYLIB_DIR)raylib/release/include \
-isystem$(RAYLIB_DIR)raylib/src/external \
-L. \
-L/usr/local/lib \
-L$(RAYLIB_DIR)raylib/src \
-lraylib \
-lGL \
-lm \
-lpthread \
-ldl \
-lrt \
-lX11 \
-lGLU \
-lXxf86vm \
-lXrandr \
-lXinerama \
-lXcursor \
qif/qif.a

# -lraylib -lglfw3 -lX11 -lXxf86vm -lXrandr -ldl -lXinerama -lXcursor -lGL -lGLU -lpthread -lm -std=c99 -Wall

all: main

main: graphics.o
	g++ main.cpp $(CFLAGS) bin/graphics.o

graphics.o:
	g++ -c src/graphics.cpp
	mv graphics.o bin/

clean:
	rm -f *.o
	rm -f *.out