// EarthShader.cpp

#include <GLM\gtc\matrix_transform.hpp>

#include "EarthShader.h"

EarthShader::EarthShader(GLContext& context, glm::mat4& modelMatrix) : GLShader(
	context,
	{
		{
#include "EarthShader.vs.glsl"
			,
			GL_VERTEX_SHADER,
			"Vertex Earth Shader"
		},
		{
#include "EarthShader.fs.glsl"
			,
			GL_FRAGMENT_SHADER,
			"Fragment Earth Shader"
		}
	}), modelMatrix_(modelMatrix)
{
	dayTexture_ = new GLTexture("res\\textures\\earthDay.jpg");
	nightTexture_ = new GLTexture("res\\textures\\earthNight.jpg");
	specularTexture_ = new GLTexture("res\\textures\\earthSpecular.jpg");
}

void EarthShader::prepare()
{
	int width = getContext()->getDisplay().getWidth();
	int height = getContext()->getDisplay().getHeight();
	glm::mat4 perspectiveMatrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, -1.0f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	uniformMat4("P", perspectiveMatrix);
	uniformMat4("V", viewMatrix);
	uniformMat4("M", modelMatrix_);

	dayTexture_->start(0);
	uniformInt("dayTexture", 0);

	nightTexture_->start(1);
	uniformInt("nightTexture", 1);

	specularTexture_->start(2);
	uniformInt("specularTexture", 2);
}