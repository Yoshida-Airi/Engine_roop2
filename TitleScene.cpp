#include "TitleScene.h"
#include"SceneManager.h"
#include"GamePlayScene.h"

TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	input = Input::GetInstance();

}

void TitleScene::Update()
{
	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{

		BaseScene* scene = new GamePlayScene(sceneManager_);
		sceneManager_->IsNextScene(scene);
	}

}

void TitleScene::Draw()
{

}

