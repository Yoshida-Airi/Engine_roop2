#include "MyGame.h"

#include"TitleScene.h"

MyGame::~MyGame()
{
	delete scene;
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	Framework::Initialize();
	scene = new TitleScene(sceneManager_);
	//シーンマネージャーに最初のシーンをセット
	sceneManager_->IsNextScene(scene);
	
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
