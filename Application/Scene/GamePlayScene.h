#pragma once

#include"BaseScene.h"
#include"SceneManager.h"
#include"Input.h"

#include"Sprite.h"
#include"Camera.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene:public BaseScene
{
public:
	~GamePlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;
	Input* input;

	//画像保存用
	uint32_t uvTexture;
	uint32_t Doll;

	Camera* camera;
	
	//スプライト
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;

};

