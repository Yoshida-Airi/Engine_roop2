#include "MyGame.h"

MyGame::~MyGame()
{

}

void MyGame::Initialize()
{
	
	Framework::Initialize();
	scene_ = new Game;
	scene_->Initialize();
}

void MyGame::Update()
{
	/*--- ゲームループ  ---*/

	Framework::Update();
	scene_->Update();
}

void MyGame::Draw()
{
	Framework::Draw();
	scene_->Draw();

	Framework::PostDraw();

}
