#pragma once
#include"Framework.h"
#include"Game.h"
#include"Title.h"
#include"IScene.h"

class MyGame : public Framework
{
public:
	~MyGame();
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:


	
};

