#pragma once
#include"ImGuiManager.h"

#include"Model.h"
#include"Sprite.h"
#include"ICamera.h"
#include"Camera.h"

#include"Input.h"
#include"TextureManager.h"

#include"PlayerBullet.h"

class Player
{
public:

	~Player();
	void Initialize(ICamera* camera, ICamera* UICamera, ModelData player, ModelData bullet);
	void Update();
	void Draw();
	void DrawUI();

	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; };

	Vector3 GetWorldPosition();

	/// <summary>
	/// レティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetReticleWorldPosition();

private:

	void Move();
	void Rotate();
	void Attack();
	void SetReticle();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	

	Input* input;
	TextureManager* texture;
	ICamera* camera_;
	ICamera* UICamera_;
	ModelLoader* object;

	ModelData playerData_;
	ModelData bulletData_;
	ModelData reticleData_;

	Model* player = nullptr;
	//2Dレティクル用スプライト
	uint32_t reticle;
	Sprite* sprite2DReticle_ = nullptr;

	//3Dレティクル用
	Model* reticle3DModel = nullptr;


	// 弾
	std::list<PlayerBullet*> bullets_;


	
};

