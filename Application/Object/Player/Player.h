#pragma once
#include"Model.h"
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

	Vector3 GetWorldPosition()override;

	void OnCollision()override;

	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; };

	void SetParent(const WorldTransform* transform);

private://メンバ変数
	Input* input_ = nullptr;

	std::unique_ptr<Model> playerModel_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	std::unique_ptr<Model> reticleModel = nullptr;

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

