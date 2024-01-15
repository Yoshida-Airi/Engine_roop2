#pragma once
#include"Model.h"

class Player
{
public:
	void Initialize(ModelData playerData);
	void Update();
	void Draw(ICamera* camera);

private:
	std::unique_ptr<Model> player = nullptr;
};

