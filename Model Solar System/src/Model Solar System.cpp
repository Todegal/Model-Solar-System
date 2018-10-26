#include "util\log.h"

#include "opengl\GLContext.h"
#include "opengl\GLModel.h"
#include "opengl\GLFont.h"

#include "Display.h"
#include "Scene.h"

#include "MSS\shaders\TestShader.h"
#include "MSS\SphereFactory.h"
#include "MSS\MainMenu\MainMenu.h"

#ifdef _DEBUG
Display display(0, 0, "Model Solar System", false);
#else
Display display(0, 0, "Model Solar System", true);
#endif

GLContext context(display);

bool isRunning = false;

Scene* currentScene;

double currTime, lastTime, deltaTime;

void loadScene(Scene* scene)
{
	if (scene == nullptr)
	{
		ERROR("INVALID SCENE");
	}

	scene->load(&context);

	currentScene = scene;
}

void init()
{
	isRunning = true;

	currTime = timer::timeSinceInit<double>();
	lastTime = currTime;
	deltaTime = 0;

	loadScene(new MainMenu());
}

void update()
{
	display.pollEvents();

	if (display.shouldClose())
	{
		isRunning = false;
	}

	Scene* nS = currentScene->getNextScene();
	if (nS != nullptr)
	{
		loadScene(nS);
	}

	currentScene->update(deltaTime);
}

void render()
{
	context.clear();

	currentScene->render();

	display.swapBuffers();
}

void run()
{
	init();

	double lastSecond = 0;

	int frames = 0;

	while (isRunning)
	{
		lastTime = currTime;
		currTime = timer::timeSinceInit<double>();
		deltaTime = (currTime - lastTime) / 1000;

		update();

		render();
		frames += 1;

		if (currTime - lastSecond >= 1000)
		{
			MESSAGE("ms/frame: " + std::to_string(1 / (double)frames * 1000));
			lastSecond += 1000;
			frames = 0;
		}
	}
}

int main(int argc, char** argv)
{
	try
	{
		run();
	}
	catch (logger::LoggerException &e)
	{
		system("pause");
		return -1;
	}

	return 1;
}