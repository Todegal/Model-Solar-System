R"(
#version 330 core

in vec3 fragPos;

out vec3 color;

void main()
{
	color = vec3(1, 0, 0) * fragPos.z;
}
)"