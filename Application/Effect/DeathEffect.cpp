#include "DeathEffect.h"

void DeathEffect::Initialize(Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture = textureManager_->LoadTexture("Resources/SampleAssets/circle.png");

	Vector3 velocity = { 0.0f,0.0f,0.0f };
	hitEffect.reset(ParticleSystem::Create(circleTexture, camera, velocity, false));
	hitEffect->emitter_->transform.scale = { 1.0f,0.6f,1.0f };
	hitEffect->emitter_->count = 40;
	hitEffect->emitter_->frequency = 0.2f;
	hitEffect->SetLifeTime(1.0f, 3.0f);
	hitEffect->SetUseBillBoard();
	hitEffect->SetRandomAllVelocity();
	hitEffect->SetColor({ 1.0f,1.0f,0.0f });
	hitEffect->SetRandomColor();
}

void DeathEffect::Update()
{
	hitEffect->Debug("deathEffect");
	hitEffect->Update();

	if (flag)
	{
		playTime--;


		if (playTime <= 0)
		{
			//実行時間を過ぎたら粒の生成を止める
			hitEffect->StopMakeParticle();

			if (hitEffect->GetIsParticleEmpty())
			{
				//エフェクトの粒が0になったら消滅フラグを立てる
				isDead = true;
			}
		}
	}
}

void DeathEffect::Draw()
{
	if (flag)
	{
		hitEffect->Draw();
	}
}

bool DeathEffect::IsDead()
{
	return isDead;
}
