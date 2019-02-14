#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aColor; // Vertex color

out vec3 ourColor;

void main(){
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
}