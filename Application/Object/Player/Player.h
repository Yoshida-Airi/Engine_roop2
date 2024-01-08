#pragma once
#include"ImGuiManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"

#include"Input.h"

#include"PlayerBullet.h"

class Player
{
public:

	~Player();
	void Initialize(ICamera* camera, ModelData player, ModelData bullet);
	void Update();
	void Draw();
	Vector3 GetWorldPosition();


private:

	void Move();
	void Rotate();
	void Attack();

	
	Input* input;
	ICamera* camera_;

	ModelData playerData_;
	ModelData bulletData_;

	Model* player = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;


	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; };

};

