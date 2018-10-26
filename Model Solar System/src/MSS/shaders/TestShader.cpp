// TestShader.cpp -
// Test shader - what do you think

#include <GLM\gtc\matrix_transform.hpp>

#include "TestShader.h"

TestShader::TestShader(GLContext& context, glm::mat4& modelMatrix) : GLShader(context, { {
#include "testShader.vs.glsl"
		,
		GL_VERTEX_SHADER,
		"Test Vertex Shader"
	} ,
	{
#include "testShader.fs.glsl"
		,
		GL_FRAGMENT_SHADER,
		"Test Fragment Shader"
	} }),
	modelMatrix_(modelMatrix)
{
}

void TestShader::prepare()
{
	int width = getContext()->getDisplay().getWidth();
	int height = getContext()->getDisplay().getHeight();
	glm::mat4 perspectiveMatrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	uniformMat4("P", perspectiveMatrix);
	uniformMat4("V", viewMatrix);
	uniformMat4("M", modelMatrix_);
}