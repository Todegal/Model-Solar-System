// GLShader.h -
// Wraps an opengl shader
#ifndef GLSHADER_H
#define GLSHADER_H

#include <string>
#include <vector>

#include <GL\glew.h>

#include <GLM\mat4x4.hpp>

#include "GLContext.h"

// Struct for shader creation
struct ShaderDesc
{
	std::string code;
	GLenum type;
	std::string name;
};

class GLShader
{
private:
	GLuint shaderProgram_ = 0;

	GLContext* context_ = 0;

	void release();

protected:
	GLContext * getContext() { return context_; }

	// Uniform variables
	void uniformMat4(std::string name, glm::mat4& matrix);
	void uniformInt(std::string name, int i) { glUniform1i(glGetUniformLocation(shaderProgram_, name.c_str()), i); }

public:
	GLShader(GLContext& context, std::vector<ShaderDesc> shaders);
	~GLShader() { release(); }

	void start();
	void stop();

	virtual void prepare() {}

	// Delete the copy constructor/assignment
	GLShader(const GLShader &) = delete;
	GLShader &operator=(const GLShader &) = delete;

	GLShader(GLShader &&other) : shaderProgram_(other.shaderProgram_), context_(other.context_)
	{
		other.shaderProgram_ = 0;
		other.context_ = 0;
	}

	GLShader &operator=(GLShader &&other)
	{
		// Check for self assignment
		if (this != &other)
		{
			release();

			std::swap(shaderProgram_, other.shaderProgram_);
			std::swap(context_, other.context_);
		}
	}
};

#endif // !GLSHADER_H
