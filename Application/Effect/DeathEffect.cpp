#include "DeathEffect.h"

void DeathEffect::Initialize(Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture_ = textureManager_->LoadTexture("Resources/SampleAssets/circle.png");

	Vector3 velocity = { 0.0f,0.0f,0.0f };
	hitEffect_.reset(ParticleSystem::Create(circleTexture_, camera, velocity, false));
	hitEffect_->emitter_->transform.scale = { 1.0f,0.6f,1.0f };
	hitEffect_->emitter_->count = 40;
	hitEffect_->emitter_->frequency = 0.2f;
	hitEffect_->SetLifeTime(1.0f, 3.0f);
	hitEffect_->SetUseBillBoard();
	hitEffect_->SetRandomAllVelocity();
	hitEffect_->SetColor({ 1.0f,1.0f,0.0f });
	hitEffect_->SetRandomColor();
}

void DeathEffect::Update()
{
	hitEffect_->Debug("deathEffect");
	hitEffect_->Update();

	if (flag_)
	{
		playTime_--;


		if (playTime_ <= 0)
		{
			//実行時間を過ぎたら粒の生成を止める
			hitEffect_->StopMakeParticle();

			if (hitEffect_->GetIsParticleEmpty())
			{
				//エフェクトの粒が0になったら消滅フラグを立てる
				isDead_ = true;
			}
		}
	}
}

void DeathEffect::Draw()
{
	if (flag_)
	{
		hitEffect_->Draw();
	}
}

bool DeathEffect::IsDead()
{
	return isDead_;
}
