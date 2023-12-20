#include "SceneManager.h"


SceneManager::~SceneManager()
{
	//最後のシーンの解放
	delete scene_;
}

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (nextSscene_)
	{
		//旧シーンの終了
		if (scene_)
		{
			delete scene_;
		}

		//シーン切り替え
		scene_ = nextSscene_;
		nextSscene_ = nullptr;
		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}
