#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"

#include"Input.h"



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
	
};

