// GLContext.cpp -
// Handles and verifies an opengl context

#include "..\util\log.h"

#include "GLContext.h"

#ifdef _DEBUG
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		std::stringstream ss;
		ss << "GL ERROR: type = " << type << ", severity = " << severity << ", message = " << message;
		ERROR(ss.str());
	}
	else
	{
		std::stringstream ss;
		ss << "GL CALLBACK: type = " << type << ", severity = " << severity << ", message = " << message;
		WARNING(ss.str());
	}
}
#endif

GLContext::GLContext(Display& display) :
	display_(display)
{
	glfwMakeContextCurrent(display.getPointer());

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		ERROR("Failed to initialize GLEW");
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif // DEBUG_

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	isValid_ = true;
}

GLContext::~GLContext()
{
	isValid_ = false;
}