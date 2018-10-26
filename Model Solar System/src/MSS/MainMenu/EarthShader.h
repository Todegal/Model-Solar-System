// EarthShader.h
#ifndef EARTHSHADER_H
#define EARTHSHADER_H

#include <GLM\mat4x4.hpp>

#include "..\..\opengl\GLShader.h"
#include "..\..\opengl\GLTexture.h"

class EarthShader : public GLShader
{
private:
	// Reference to the transformation matrix of the object
	glm::mat4& modelMatrix_;

	GLTexture* dayTexture_;
	GLTexture* nightTexture_;
	GLTexture* specularTexture_;
public:
	EarthShader(GLContext& context, glm::mat4& modelMatrix);

	void prepare();
};

#endif // !EARTHSHADER_H
