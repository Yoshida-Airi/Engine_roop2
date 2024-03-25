#pragma once

#include"BaseScene.h"
#include"SceneManager.h"
#include"Input.h"

#include"Camera.h"
#include"Model.h"
#include"ParticleSystem.h"

class TitleScene :public BaseScene
{
public:
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	TextureManager* texture;
	Camera* camera;

	//テクスチャ保存
	uint32_t uvTexture;
	uint32_t circle;

	//モデル
	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;

	//パーティクル
	std::unique_ptr<ParticleSystem> particle = nullptr;
	std::unique_ptr<ParticleSystem> particle2 = nullptr;
	
};

