R"(
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;

void main()
{
	fragPos = (M * vec4(vertex, 1.0)).xyz;
	gl_Position = P * V * M * vec4(vertex, 1.0);
}
)"