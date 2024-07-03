#pragma once
#include"Model.h"

class Skydome
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

private:
	std::unique_ptr<Model> skydomeModel_ = nullptr;

};
