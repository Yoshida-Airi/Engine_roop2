/**
*	@file TitleScene.h
*	@brief タイトルシーンクラスヘッダ
*/

#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"TextureManager.h"
#include"Input.h"
#include"Audio.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Camera.h"
#include"Effect/Fade.h"
#include"Effect/TitleEffect.h"
#include"ParticleSystem.h"
#include"Object/Skydome/Skydome.h"
#include"EffectManager.h"

/**
*   @class TitleScene
*	@brief タイトルシーンクラス
*/
class TitleScene :public BaseScene
{
public:
	/// @brief デストラクタ
	~TitleScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

	/// @brief スプライトの点滅の更新
	void UpdateSpriteBlink();

private:

	EffectManager* efect = nullptr;
	TextureManager* textureManager = nullptr;

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	uint32_t titleLogo;
	uint32_t backGroundTexture;
	uint32_t spaceTexture;
	uint32_t soundData;
	
	std::unique_ptr<Sprite> title = nullptr;
	std::unique_ptr<Sprite> backGround = nullptr;
	std::unique_ptr<Sprite> space = nullptr;

	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;

	std::unique_ptr <Skydome>skydome;

	std::unique_ptr<Fade> fade_ = nullptr;
	std::unique_ptr<TitleEffect> titleEffect_ = nullptr;
	
	bool isSceneChange = false;

	const float kDeltaTime = 1.0f / 60.0f;
	int frameCount = 0;
	int blinkFrames = 50;  // 30フレームごとに点滅
};

