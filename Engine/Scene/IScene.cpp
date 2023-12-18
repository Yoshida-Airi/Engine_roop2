#include "IScene.h"

IScene::IScene(SceneManager* sceneManager)
	:sceneManager_(sceneManager)
{

}

void IScene::SetSceneManager(SceneManager* sceneManager)
{
	sceneManager_ = sceneManager;
}
