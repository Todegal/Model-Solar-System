// Display.cpp -
// Manages a glfw window

#include "util/log.h"

#include "Display.h"

Display::Display(int width, int height, std::string title, bool fullscreen)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		ERROR("Could not initialise GLFW");
	}

	// Get the appropriate video settings via a video mode
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Set the colour depth
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);

	// Request multisampling
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Set the OpenGL version
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);

	// The window should not be resizable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Temp width and height
	int w, h;

	if (width == 0 && height == 0)
	{
		// If the width and height are 0 set the width and height to that of the monitor
		w = mode->width;
		h = mode->height;
	}
	else
	{
		w = width;
		h = height;
	}

	// Create the window
	window_ = glfwCreateWindow(w, h, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!window_)
	{
		ERROR("Display creation failed");
	}

	MESSAGE("Display created");
}

Display::~Display()
{
	window_ = NULL;
	glfwTerminate();

	MESSAGE("Display closed");
}

int Display::getHeight()
{
	int h;
	glfwGetWindowSize(window_, NULL, &h);
	return h;
}

int Display::getWidth()
{
	int w;
	glfwGetWindowSize(window_, &w, NULL);
	return w;
}