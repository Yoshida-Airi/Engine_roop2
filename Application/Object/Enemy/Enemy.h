#pragma once
#include"Model.h"

class Enemy
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

private:

	std::unique_ptr<Model>enemyModel;

};

