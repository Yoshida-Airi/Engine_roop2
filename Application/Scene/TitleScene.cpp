#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

}

void TitleScene::Update()
{
	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{

}

