// GLModel.h -
// Handles an opengl model
#ifndef GLMODEL_H
#define GLMODEL_H

#include <vector>

#include <GL\glew.h>

#include <GLM\vec3.hpp>
#include <GLM\vec2.hpp>

#include "..\util\log.h"

#include "GLContext.h"

// The model wrapper
class GLModel
{
private:
	GLuint vao_ = 0;

	// Enumerate the buffer indices
	enum {
		VERTEX_BUFFER,
		UV_BUFFER,
		NORMAL_BUFFER,
		ELEMENT_BUFFER,
		NUM_BUFFERS
	};

	std::vector<GLuint> buffers_;

	int numVertices_;

	void release();

public:
	GLModel(GLContext& context,
		std::vector<glm::vec3> vertices,
		std::vector<glm::vec2> uvs,
		std::vector<glm::vec3> normals,
		std::vector<int> indices);
	~GLModel() { release(); }

	void render();

	// Delete the copy constructor/assignment
	GLModel(const GLModel &) = delete;
	GLModel &operator=(const GLModel &) = delete;

	GLModel(GLModel &&other) : vao_(other.vao_), buffers_(other.buffers_), numVertices_(other.numVertices_)
	{
		other.vao_ = 0;
		other.buffers_.clear();
		other.numVertices_ = 0;
	}

	GLModel &operator=(GLModel &&other)
	{
		// Check for self assignment
		if (this != &other)
		{
			release();

			std::swap(vao_, other.vao_);
			std::swap(buffers_, other.buffers_);
			std::swap(numVertices_, other.numVertices_);
		}
	}
};

#endif // !GLMODEL_H