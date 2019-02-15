all: clean main

clean:
	rm -f *.o
	rm -f *.out

main: shader.o graphics.o imgui.o imgui_draw.o imgui_impl_glfw_gl3.o
	g++ main.cpp -o main bin/*.o qif/qif.a -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lfreetype -I/usr/local/include/freetype2 /usr/local/src/glad.o /usr/lib/x86_64-linux-gnu/libdl.so

shader.o:
	g++ -c ./src/shader.cpp
	mv shader.o ./bin

graphics.o:
	g++ -c ./src/graphics.cpp
	mv graphics.o ./bin

imgui.o:
	g++ -c ./imgui/imgui.cpp
	mv imgui.o ./bin

imgui_impl_glfw_gl3.o:
	g++ -c ./imgui/imgui_impl_glfw_gl3.cpp
	mv imgui_impl_glfw_gl3.o ./bin

imgui_draw.o:
	g++ -c ./imgui/imgui_draw.cpp
	mv imgui_draw.o ./bin