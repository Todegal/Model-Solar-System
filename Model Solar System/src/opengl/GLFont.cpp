// GLFont.cpp -
// Wrapper for rendering text

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLM\vec3.hpp>
#include <GLM\gtc\matrix_transform.hpp>

#include "..\util\log.h"

#include "GLFont.h"

GLFont::GLFont(GLContext& context, std::string fontName, float size)
{
	// Ensure there is a valid context
	if (!context.isValid())
	{
		ERROR("May not use opengl without a valid context");
	}

	context_ = &context;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		ERROR("Could not initialise FreeType fonts");
	}

	FT_Face face;
	if (FT_New_Face(ft, fontName.c_str(), 0, &face))
	{
		WARNING("Could not load font: " + fontName);
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	// Disable byte-alignment restrictions
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			WARNING("Failed to load Glyph: " + std::to_string(c));
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Char character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters_.insert(std::pair<GLchar, Char>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Buffer the shader code into the shader
	const GLchar* source = (const GLchar*)(
#include "fontShader.vs.glsl"
		);
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the shader
	glCompileShader(vertexShader);

	// ERROR CHECKING
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		WARNING(infoLog.data());
	}
	else
	{
		std::stringstream ss;
		ss << "Compiled shader: " << "Vertex Font Shader";
		MESSAGE(ss.str());
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Buffer the shader code into the shader
	source = (const GLchar*)(
#include "fontShader.fs.glsl"
		);
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the shader
	glCompileShader(fragmentShader);

	// ERROR CHECKING
	isCompiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);

		WARNING(infoLog.data());
	}
	else
	{
		std::stringstream ss;
		ss << "Compiled shader: " << "Fragment Font Shader";
		MESSAGE(ss.str());
	}

	shaderProgram_ = glCreateProgram();

	// Attach the shaders
	glAttachShader(shaderProgram_, vertexShader);
	glAttachShader(shaderProgram_, fragmentShader);

	// Verify all shaders were correctly attached
	int count = 0;
	glGetProgramiv(shaderProgram_, GL_ATTACHED_SHADERS, &count);
	if (count != 2)
	{
		WARNING("INCORRECT NUMBER OF ATTACHED SHADERS! Expected : " + std::to_string(2) + " Recieved : " + std::to_string(count));
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

	glDetachShader(shaderProgram_, vertexShader);
	glDetachShader(shaderProgram_, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create buffer
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GLFont::render(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	glUseProgram(shaderProgram_);

	// Generate orthogonal perspective matrix
	glm::mat4 projection = glm::ortho(0.0f, (float)context_->getDisplay().getWidth(),
		0.0f, (float)context_->getDisplay().getHeight());
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram_, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniform3f(glGetUniformLocation(shaderProgram_, "textColor"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram_, "text"), 0);
	glBindVertexArray(vao_);

	float width = 0;

	// Iterate through the string and render each character
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Char ch = characters_[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFont::release()
{
	glDeleteProgram(shaderProgram_);
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);
}