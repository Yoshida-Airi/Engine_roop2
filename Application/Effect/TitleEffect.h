#pragma once
#include"ParticleSystem.h"

class TitleEffect
{
public:
	void Initialize(Camera* camera);
	void Update();
	void Draw();

	/// <summary>
	/// 実行するかどうか
	/// </summary>
	/// <param name="isFlag"></param>
	void SetFlag(bool isFlag)
	{
		flag = isFlag;
	}

	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		hitEffect->emitter_->transform.translate.x = position.x;
		hitEffect->emitter_->transform.translate.y = position.y + 0.7f;
		hitEffect->emitter_->transform.translate.z = position.z;
	}

	/// <summary>
	/// 実行が終わったかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDead();

private:

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<ParticleSystem> hitEffect = nullptr;
	uint32_t circleTexture;

	bool flag;	//実行中かどうかのフラグ
	int playTime = 20;
	bool isDead = false;


};
