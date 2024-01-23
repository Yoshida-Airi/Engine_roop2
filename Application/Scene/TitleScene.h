#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Input.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"

#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"

#include"Particle.h"


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
	ICamera* camera;
	ICamera* uiCamera;


	uint32_t  circle;

	Particle* particle = nullptr;



	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;
	
};

