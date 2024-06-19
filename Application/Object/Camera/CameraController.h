#pragma once
#include"Camera.h"


class Player;
class CameraController
{
public:
	void Initialize();
	void Update();
	//void Draw();

	//**setter**//
	void SetTarget(Player* target) { target_ = target; };
	void Reset();

private:
	Camera* camera_;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = { 0,0,-15.0f };

};

