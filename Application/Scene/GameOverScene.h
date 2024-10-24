/**
*	@file GameOverScene.h
*	@brief ゲームオーバーシーンクラスヘッダ
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

#include"EffectManager.h"

/**
*   @class GameOverScene
*	@brief  ゲームオーバーシーンクラス
*/
class GameOverScene :public BaseScene
{
public:
	/// @brief デストラクタ
	~GameOverScene()override;
	/// @brief 初期化処理
	void Initialize()override;
	/// @brief 更新処理
	void Update()override;
	/// @brief 描画処理
	void Draw()override;

private:

	EffectManager* efect = nullptr;
	TextureManager* textureManager = nullptr;

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	uint32_t titleTexture;

	uint32_t soundData;

	std::unique_ptr<Sprite> title = nullptr;

	std::unique_ptr<Model> fence_ = nullptr;
	std::unique_ptr<Model> cube_ = nullptr;



};

