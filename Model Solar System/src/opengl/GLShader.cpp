// GLShader.cpp -
// Wrapper for handling a shader

#include "..\util\log.h"

#include "GLShader.h"

GLShader::GLShader(GLContext& context, std::vector<ShaderDesc> shaders)
{
	if (!context.isValid())
	{
		ERROR("May not use opengl without an initialized context");
	}

	context_ = &context;

	// Temporary shader
	std::vector<GLuint> tempShaders;

	for (size_t i = 0; i < shaders.size(); i++)
	{
		GLuint shader = glCreateShader(shaders[i].type);

		// Buffer the shader code into the shader
		const GLchar* source = (const GLchar*)(shaders[i].code.c_str());
		glShaderSource(shader, 1, &source, 0);

		// Compile the shader
		glCompileShader(shader);

		// ERROR CHECKING
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			WARNING(infoLog.data());
		}
		else
		{
			std::stringstream ss;
			ss << "Compiled shader: " << shaders[i].name;
			MESSAGE(ss.str());
			tempShaders.push_back(shader);
		}
	}

	shaderProgram_ = glCreateProgram();

	// Attach the shaders
	for (size_t i = 0; i < tempShaders.size(); i++)
	{
		glAttachShader(shaderProgram_, (tempShaders[i]));
	}

	// Verify all shaders were correctly attached
	int count = 0;
	glGetProgramiv(shaderProgram_, GL_ATTACHED_SHADERS, &count);
	if (count != shaders.size())
	{
		WARNING("INCORRECT NUMBER OF ATTACHED SHADERS! Expected : " + std::to_string(shaders.size()) + " Recieved : " + std::to_string(count));
	}

	// Attempt to link the program
	glLinkProgram(shaderProgram_);

	// ERROR CHECKING
	GLint isLinked = 0;
	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram_, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram_, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(shaderProgram_);

		std::stringstream ss;
		ss << "Linking " << infoLog.data();
		WARNING(ss.str());
	}

	// Cleanup
	for (size_t i = 0; i < tempShaders.size(); i++)
	{
		glDetachShader(shaderProgram_, tempShaders[i]);
	}
	for (size_t i = 0; i < tempShaders.size(); i++)
	{
		glDeleteShader(tempShaders[i]);
	}
}

void GLShader::release()
{
	glDeleteProgram(shaderProgram_);
}

void GLShader::start()
{
	glUseProgram(shaderProgram_);
	prepare();
}

void GLShader::stop()
{
	glUseProgram(0);
}

void GLShader::uniformMat4(std::string name, glm::mat4& matrix)
{
	GLint loc = glGetUniformLocation(shaderProgram_, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
}