#include "MyGame.h"

#include"TitleScene.h"

MyGame::~MyGame()
{
	delete scene_;
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();
	scene_ = new TitleScene();
	scene_->Initialize();
}

void MyGame::Update()
{

	/*--- ゲームループ  ---*/

	Framework ::Update();
	
}

void MyGame::Draw()
{
	Framework::Draw();
}
