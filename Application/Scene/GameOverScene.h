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

#include"EfectManager.h"

class GameOverScene :public BaseScene
{
public:
	~GameOverScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	EfectManager* efect = nullptr;
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

