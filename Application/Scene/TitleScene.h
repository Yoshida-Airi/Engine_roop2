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

#include"EfectManager.h"

class TitleScene :public BaseScene
{
public:
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	//EfectManager* efect = nullptr;

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	ICamera* camera;
	ICamera* uiCamera;
	ModelLoader* object;
	ModelData fenceData;
	ModelData cubeData;


	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;

	
	
};

