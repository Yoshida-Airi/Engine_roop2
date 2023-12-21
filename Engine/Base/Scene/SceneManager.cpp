#include "SceneManager.h"
#include<cassert>

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager;
	}
	return instance;
}

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

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextSscene_ == nullptr);

	//次シーンを生成
	nextSscene_ = sceneFactory_->CreateScene(sceneName);
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory)
{
	sceneFactory_ = sceneFactory;
}


//静的メンバ変数の宣言と初期化
SceneManager* SceneManager::instance = nullptr;