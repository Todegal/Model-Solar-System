// GLContext.h -
// Creates and verifies an opengl context
#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <GL\glew.h>

#include "..\Display.h"

#ifdef _DEBUG
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);
#endif

class GLContext
{
private:
	bool isValid_;

	Display& display_;

public:
	GLContext(Display& display);
	~GLContext();

	bool isValid() { return isValid_; }

	Display& getDisplay() { return display_; }

	void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
};

#endif // !GLCONTEXT_H
