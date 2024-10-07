#include "TitleEffect.h"


void TitleEffect::Initialize(Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture = textureManager_->LoadTexture("Resources/SampleAssets/circle.png");

	Vector3 velocity = { 0.0f,5.0f,0.0f };
	hitEffect.reset(ParticleSystem::Create(circleTexture, camera, velocity, true));
	//hitEffect->emitter_->transform.translate = { 0.0f,-2.1f,0.0f };
	hitEffect->emitter_->transform.scale = { 11.0f,2.0f,1.0f };
	hitEffect->emitter_->count = 60;
	hitEffect->emitter_->frequency = 0.2f;
	hitEffect->SetLifeTime(1.0f, 3.0f);
	
	hitEffect->SetUseBillBoard();
	//hitEffect->SetRandomAllVelocity();
	hitEffect->SetRandomVelocityY(true);
	hitEffect->SetRandomVelocityZ(true);
	hitEffect->SetColor({ 1.0f,1.0f,1.0f });
	
	//hitEffect->SetRandomColor();
}

void TitleEffect::Update()
{
	hitEffect->Debug("Effect");
	hitEffect->Update();

	//if (flag)
	//{
	//	playTime--;


	//	if (playTime <= 0)
	//	{
	//		//実行時間を過ぎたら粒の生成を止める
	//		hitEffect->StopMakeParticle();

	//		if (hitEffect->GetIsParticleEmpty())
	//		{
	//			//エフェクトの粒が0になったら消滅フラグを立てる
	//			isDead = true;
	//		}
	//	}
	//}
}

void TitleEffect::Draw()
{
	//if (flag)
	//{
		hitEffect->Draw();
	//}
}

bool TitleEffect::IsDead()
{
	return isDead;
}
