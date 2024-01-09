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

#include"Player.h"

class GameOverScene :public BaseScene
{
public:
	~GameOverScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	TextureManager* texture = nullptr;
	ICamera* camera;
	ICamera* uiCamera;
	ModelLoader* object;
	ModelData fenceData;

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;


	uint32_t uvTexture;

	Sprite* sprite = nullptr;

};

