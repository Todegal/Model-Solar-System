// MainMenu.cpp

#include "MainMenu.h"

#include <GLM\vec3.hpp>
#include <GLM\gtc\matrix_transform.hpp>

void MainMenu::load(GLContext* context)
{
	consolas_ = new GLFont(*context, "res/fonts/CONSOLA.ttf", 48);
	consolasBig_ = new GLFont(*context, "res/fonts/CONSOLA.ttf", 96);

	startButtonColour_ = glm::vec3(1, 1, 1);
	quitButtonColour_ = glm::vec3(1, 1, 1);

	subtextMidPoint_ = glm::vec2(0, 0);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;

	generateSphere(1.0f, 32, 32, vertices, uvs, normals, indices);
	earth_ = new GLModel(*context, vertices, uvs, normals, indices);

	earthTransformation_ = glm::mat4(1.0);
	earthTransformation_ = glm::rotate(earthTransformation_, glm::pi<float>(), glm::vec3(1, 0, 0));
	earthTransformation_ = glm::rotate(earthTransformation_, glm::radians(30.0f), glm::vec3(1, 0, 0));

	earthShader_ = new EarthShader(*context, earthTransformation_);

	Scene::load(context);
}

void MainMenu::render()
{
	earthShader_->start();

	earth_->render();

	earthShader_->stop();

	consolasBig_->render("Model Solar System",
		(context_->getDisplay().getWidth() / 2) - 450,
		(context_->getDisplay().getHeight() / 2) - 100,
		1.0f, glm::vec3(1, 1, 1));
	consolas_->render("Start Simulation",
		(context_->getDisplay().getWidth() / 2) - 200,
		(context_->getDisplay().getHeight() / 2) - 200,
		1.0f, startButtonColour_);
	consolas_->render("Quit",
		(context_->getDisplay().getWidth() / 2) - 50,
		(context_->getDisplay().getHeight() / 2) - (200 + 48),
		1.0f, quitButtonColour_);

	consolas_->render("By Niall Townley",
		(context_->getDisplay().getWidth() / 2) - 100,
		(context_->getDisplay().getHeight() / 2) - 135,
		0.5f, glm::vec3(0.8f, 0.8f, 0.8f));
}

void MainMenu::update(double deltaTime)
{
	double mx, my;
	glfwGetCursorPos(context_->getDisplay().getPointer(), &mx, &my);

	bool xbounds = (context_->getDisplay().getWidth() / 2) - 200 < mx && mx < (context_->getDisplay().getWidth() / 2) + 225;
	bool ybounds = (context_->getDisplay().getHeight() / 2) + 150 < my && my < (context_->getDisplay().getHeight() / 2) + 200;
	if (xbounds && ybounds)
	{
		startButtonColour_ = glm::vec3(0, 0, 1);
	}
	else
	{
		startButtonColour_ = glm::vec3(1, 1, 1);
	}

	xbounds = (context_->getDisplay().getWidth() / 2) - 50 < mx && mx < (context_->getDisplay().getWidth() / 2) + 50;
	ybounds = (context_->getDisplay().getHeight() / 2) + 210 < my && my < (context_->getDisplay().getHeight() / 2) + 250;
	if (xbounds && ybounds)
	{
		quitButtonColour_ = glm::vec3(1, 0, 0);
		if (glfwGetMouseButton(context_->getDisplay().getPointer(), GLFW_MOUSE_BUTTON_1))
		{
			glfwSetWindowShouldClose(context_->getDisplay().getPointer(), true);
		}
	}
	else
	{
		quitButtonColour_ = glm::vec3(1, 1, 1);
	}

	earthTransformation_ = glm::rotate(earthTransformation_, 0.1f * (float)deltaTime, glm::vec3(0, 1, 0));
}