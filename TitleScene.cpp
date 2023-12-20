#include "TitleScene.h"

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


}

void TitleScene::Draw()
{

}

