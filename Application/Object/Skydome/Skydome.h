#pragma once
#include"Model.h"

class Skydome
{
public:
	void Initialize();
	void Update();
	void Draw(ICamera* camera);

private:
	std::unique_ptr<Model> skydomeModel_ = nullptr;

};

