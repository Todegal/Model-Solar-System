// GLModel.cpp -
// Handles an opengl model

#include "GLModel.h"

GLModel::GLModel(GLContext& context,
	std::vector<glm::vec3> vertices,
	std::vector<glm::vec2> uvs,
	std::vector<glm::vec3> normals,
	std::vector<int> indices)
{
	// Check that the context is valid
	if (!context.isValid())
	{
		ERROR("May not use opengl without an initialized context");
	}

	// Store the number of vertices
	numVertices_ = indices.size();

	// Generate the vao
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Genereate the buffers
	GLuint b[NUM_BUFFERS];
	glGenBuffers(NUM_BUFFERS, b);

	buffers_ = std::vector<GLuint>(b, b + NUM_BUFFERS);

	// Fill the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glDisableVertexAttribArray(0);

	// Fill the uv buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[UV_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glDisableVertexAttribArray(1);

	// Fill the normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glDisableVertexAttribArray(2);

	// Fill the elements buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[ELEMENT_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the vao
	glBindVertexArray(0);

	MESSAGE("Created model");
}

void GLModel::render()
{
	glBindVertexArray(vao_);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, numVertices_, GL_UNSIGNED_INT, (void*)0);
}

void GLModel::release()
{
	glDeleteBuffers(NUM_BUFFERS, buffers_.data());
	glDeleteVertexArrays(1, &vao_);
}