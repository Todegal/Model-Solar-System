// MainMenu.h
#ifndef MAINMENU_H
#define MAINMENU_H

#include "..\..\Scene.h"

#include "..\..\opengl\GLFont.h"
#include "..\..\opengl\GLModel.h"
#include "..\SphereFactory.h"
#include "EarthShader.h"

class MainMenu : public Scene
{
public:
	void load(GLContext* context);

	void update(double deltaTime);
	void render();

private:
	GLFont * consolas_;
	GLFont * consolasBig_;

	glm::mat4 earthTransformation_;

	glm::vec3 quitButtonColour_;
	glm::vec3 startButtonColour_;

	glm::vec2 subtextMidPoint_;

	EarthShader * earthShader_;
	GLModel * earth_;
};

#endif // !MAINMENU_H
