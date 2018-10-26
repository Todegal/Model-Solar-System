// TestShader.h -
// test shader for demonstrating tech
#ifndef TESTSHADER_H
#define TESTSHADER_H

#include <GLM\mat4x4.hpp>

#include "..\..\opengl\GLShader.h"

class TestShader : public GLShader
{
private:
	// Reference to the transformation matrix of the object
	glm::mat4& modelMatrix_;
public:
	TestShader(GLContext& context, glm::mat4& modelMatrix);

	void prepare();
};

#endif // !TESTSHADER_H
