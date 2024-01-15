#pragma once
#include"Model.h"

class PlayerBullet
{
public:
	void Initialize(ModelData bulletData, Vector3 pos);
	void Update();
	void Draw(ICamera* camera);

private:

	std::unique_ptr<Model> bullet = nullptr;
};

