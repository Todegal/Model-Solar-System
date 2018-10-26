// Scene.h -
// Defines a scene
#ifndef SCENE_H
#define SCENE_H

#include "opengl\GLContext.h"

class Scene
{
public:
	Scene() : loaded_(false) {}

	virtual void render() = 0;
	virtual void update(double deltaTime) = 0;

	virtual void load(GLContext* context)
	{
		loaded_ = true;
		nextScene_ = nullptr;
		context_ = context;
	}

	bool getLoaded() { return loaded_; }

	Scene* getNextScene() {
		Scene* s = nextScene_;
		nextScene_ = nullptr;
		return s;
	}

protected:
	template<typename T>
	void setNextScene()
	{
		nextScene_ = new T();
	}

	GLContext* context_;

private:
	bool loaded_;
	Scene* nextScene_;
};

#endif // !SCENE_H
