// Display.h -
// Manages a glfw window
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

#include <GLFW\glfw3.h>

// Class for creating and setting up a display
class Display
{
private:
	// Window pointer
	GLFWwindow * window_ = NULL;

public:
	Display(int width, int height, std::string title, bool isFullscreen);
	~Display();

	int getWidth();
	int getHeight();

	bool shouldClose() { return glfwWindowShouldClose(window_); }

	void pollEvents() { glfwPollEvents(); }
	void swapBuffers() { glfwSwapBuffers(window_); }

	GLFWwindow* getPointer() { return window_; }
};

#endif // !DISPLAY_H
