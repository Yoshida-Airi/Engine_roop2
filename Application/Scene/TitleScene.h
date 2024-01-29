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



class TitleScene :public BaseScene
{
public:
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void InitializeFloatingGimmick();

	void UpAndDownMotion(float time);

	void StartFadeIn();

	void UpdateFadeIn();

private:

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	ICamera* camera;
	ICamera* uiCamera;


	uint32_t titleTex;
	std::unique_ptr<Sprite>titleSprite = nullptr;
	uint32_t ABottonTex;
	std::unique_ptr<Sprite>ABottonSprite = nullptr;
	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;


	//浮遊ギミックの媒介変数
	float UpdownParameter_ = 0.0f;
	float swingParameter_ = 0.0f;
	float throwParameter_ = 0.0f;
	//浮遊移動のサイクル<frame>
	uint16_t cycle_ = 60;
	//浮遊の振動<m>
	float amplitude_ = 0.1f;
	float swing_ = 0.1f;
	float throw_ = 0.1f;
	bool isMove = false;

	float flashTime = 0.0f;

	float alpha = 0;
	bool isFadingIn = false;
	
};

