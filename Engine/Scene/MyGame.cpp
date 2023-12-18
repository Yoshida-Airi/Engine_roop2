#include "MyGame.h"

MyGame::~MyGame()
{

}

void MyGame::Initialize()
{
	
	Framework::Initialize();

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
