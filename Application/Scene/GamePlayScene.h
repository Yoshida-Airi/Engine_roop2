#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Triangle.h"
#include"ParticleSystem.h"
#include"Sphere.h"
#include"Model.h"
#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"

#include"Input.h"
#include"Sprite.h"

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

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;
	uint32_t circle;

	ModelLoader* object;
	ModelData plane;
	ModelData cube;

	ICamera* camera;
	ICamera* uiCamera;

	Input* input;

	Emitter emitter;

	std::unique_ptr<Triangle> triangle = nullptr;
	std::unique_ptr<Triangle> triangle2 = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<ParticleSystem> sprite2 = nullptr;
	std::unique_ptr<Sphere> sphere = nullptr;
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> model2 = nullptr;


#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG


};

