#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"
#include"Input.h"
#include"SceneManager.h"

class GameClearScene :public BaseScene
{
public:
	~GameClearScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:

	Input* input;
	SceneManager* sceneManager_ = nullptr;

	ICamera* camera;
	ICamera* uiCamera;


	uint32_t overTex;
	std::unique_ptr<Sprite>overSprite = nullptr;

};

