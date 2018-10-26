// GLTexture.h -
// Wrapper for loading textures with opengl
#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <string>

#include <GL\glew.h>

class GLTexture
{
private:
	GLuint textureId_ = 0;

	void release();

public:
	GLTexture(std::string filename);
	~GLTexture() { release(); }

	void start(int id);

	GLTexture(const GLTexture&) = delete;
	GLTexture &operator=(const GLTexture&) = delete;

	GLTexture(GLTexture &&other) : textureId_(other.textureId_)
	{
		other.textureId_ = 0;
	}

	GLTexture &operator=(GLTexture &&other)
	{
		// check for self assignment
		if (this != &other)
		{
			release();

			std::swap(textureId_, other.textureId_);
		}
	}
};

#endif // !GLTEXTURE_H
