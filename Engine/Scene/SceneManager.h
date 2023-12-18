#pragma once
#include"IScene.h"
class SceneManager
{
public:

	~SceneManager();
	void Update();
	void Draw();

	void SetNextScene(IScene* nextScene);
private:
	IScene* scene_ = nullptr;
	IScene* nextScene_ = nullptr;
};

