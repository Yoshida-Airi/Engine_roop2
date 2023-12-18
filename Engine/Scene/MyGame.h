#pragma once
#include"Framework.h"
#include"Game.h"

class MyGame : public Framework
{
public:
	~MyGame();
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:

	Game* scene_ = nullptr;

	
};

