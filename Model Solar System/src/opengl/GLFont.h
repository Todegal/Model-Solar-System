// GLFont.h -
// Wrapper for handling text rendering
#ifndef GLFONT_H
#define GLFONT_H

#include <string>
#include <map>

#include <GL\glew.h>

#include <GLM\vec2.hpp>

#include "GLContext.h"

// Struct for handling individual characters
struct Char
{
	GLuint textureID; // Id of the texture with the glyph
	glm::ivec2 size; // The size of the glyph
	glm::ivec2 bearing; // Offset from the baseline
	int advance; // How far to offset for the next glyph
};

class GLFont
{
private:
	// Map of characters and their appropriate texture
	std::map<GLchar, Char> characters_;

	GLuint shaderProgram_;

	GLuint vao_;
	GLuint vbo_;

	GLContext* context_;

	void release();
public:
	GLFont(GLContext& context, std::string fontName, float size);
	~GLFont() { release(); }

	void render(std::string text, float x, float y, float scale, glm::vec3 colour);

	// Delete the copy constructor/assignment
	GLFont(const GLFont&) = delete;
	GLFont &operator=(const GLFont&) = delete;

	GLFont(GLFont&& other) : characters_(other.characters_),
		shaderProgram_(other.shaderProgram_),
		vao_(other.vao_), vbo_(other.vbo_),
		context_(other.context_)
	{
		other.shaderProgram_ = 0;
		other.vao_ = 0;
		other.vbo_ = 0;
		other.context_ = 0;
	}

	GLFont &operator=(GLFont &&other)
	{
		// Check for self assignment
		if (this != &other)
		{
			release();

			std::swap(characters_, other.characters_);
			std::swap(vao_, other.vao_);
			std::swap(vbo_, other.vbo_);
			std::swap(context_, other.context_);
			std::swap(shaderProgram_, other.shaderProgram_);
		}
	}
};

#endif // !GLFONT_H
