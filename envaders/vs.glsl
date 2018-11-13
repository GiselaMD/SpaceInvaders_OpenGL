#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform vec3 sumPos2;

void main() {
	gl_Position = vec4(vertex_position+sumPos2, 1.0);
}