#pragma once
#include"TextureManager.h"
#include"ModelLoader.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Model.h"
#include"ICamera.h"
#include"Camera.h"
#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Input.h"
#include"UICamera.h"

#include"IScene.h"
#include"Title.h"

class Game :public IScene
{
public:
	~Game();
	Game(SceneManager* sceneManager);
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:

	TextureManager* texture;
	ImGuiManager* imgui;
	
	ICamera* camera;
	ICamera* uiCamera;

	Input* input;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;

	ModelLoader* object;
	ModelData plane;
	ModelData cube;

	Model* model;
	Model* model2;
	Triangle* triangle;
	Triangle* triangle2;
	Sprite* sprite;
	Sprite* sprite2;
	Sphere* sphere;
};

