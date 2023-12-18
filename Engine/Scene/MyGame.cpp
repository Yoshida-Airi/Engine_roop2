#include "MyGame.h"

MyGame::~MyGame()
{

}

void MyGame::Initialize()
{
	
	Framework::Initialize();

	IScene* scene = new Title(sceneManager_);
	sceneManager_->SetNextScene(scene);

}

void MyGame::Update()
{
	/*--- ゲームループ  ---*/

	Framework::Update();

}

void MyGame::Draw()
{
	Framework::Draw();


}
