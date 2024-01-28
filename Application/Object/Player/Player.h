#pragma once
#include"Model.h"
#include"Sprite.h"
#include"Input.h"
#include"PlayerBullet.h"
#include"Collider.h"
#include<list>

class Player :public Collider
{
public:
	~Player();
	void Initialize(Vector3 pos);
	void Update();
	void Draw(ICamera* camera);
	void DrawUI(ICamera* camera);

	Vector3 GetWorldPosition()override;

	void OnCollision()override;

	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; };

	void SetParent(const WorldTransform* transform);

private://メンバ変数
	Input* input_ = nullptr;

	uint32_t textureReticle = TextureManager::GetInstance()->LoadTexture("Resources/reticle.png");

	std::unique_ptr<Model> playerModel_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	std::unique_ptr<Model> reticleModel = nullptr;
	std::unique_ptr<Sprite> sprite2DReticle_ = nullptr;

private://メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転（旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();



	
};

