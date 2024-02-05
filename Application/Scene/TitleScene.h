#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Input.h"

#include"Triangle.h"
#include"ParticleSystem.h"
#include"Sphere.h"
#include"Model.h"
#include"Sprite.h"


#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"


class TitleScene :public BaseScene
{
public:
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	Input* input;
	TextureManager* texture = nullptr;
	SceneManager* sceneManager_ = nullptr;
	ICamera* camera;
	ICamera* uiCamera;
	
	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Enter;
	uint32_t circle;

	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;

	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;

	Emitter emitter;
	std::unique_ptr<ParticleSystem> particle = nullptr;
	
};

