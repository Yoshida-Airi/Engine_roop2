#include "SceneManager.h"

SceneManager::~SceneManager()
{
	delete scene_;
}

void SceneManager::Update()
{
	if (nextScene_)
	{
		if (scene_)
		{
			delete scene_;
		} 
	
		scene_ = nextScene_;
		nextScene_ = nullptr;

		scene_->SetSceneManager(this);

		scene_->Initialize();

	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::SetNextScene(IScene* nextScene)
{
	nextScene_ = nextScene;
}
