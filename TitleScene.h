#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
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
	TitleScene(SceneManager* sceneManager);
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	Input* input;
	
};

