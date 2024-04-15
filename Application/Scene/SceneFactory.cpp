#include "SceneFactory.h"
#include"GamePlayScene.h"
#include"TitleScene.h"
#include"TestPlayScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlayScene();
	}
	else if (sceneName == "TEST")
	{
		newScene = new TestPlayScene();
	}
	

	return newScene;
}


