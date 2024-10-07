#pragma once
#include"Model.h"

class Skydome
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void SetLight(bool isLight)
	{
		skydomeModel_->SetLight(isLight);
	}

private:
	std::unique_ptr<Model> skydomeModel_ = nullptr;

};
