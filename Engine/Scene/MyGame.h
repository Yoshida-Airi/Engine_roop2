#pragma once
//#include"WinApp.h"
//#include"DirectXCommon.h"
//#include"ImGuiManager.h"
//#include"TextureManager.h"
#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ICamera.h"
#include"Input.h"
#include"Camera.h"
#include"UICamera.h"

#include"Framework.h"

class MyGame : public Framework
{
public:
	~MyGame();
	void Initialize()override;
	void Update()override;
	void Draw()override;

	bool IsEndRequest() { return endRequest; };

private:

	bool endRequest = false;

	//WinApp* winApp;
	//DirectXCommon* dxCommon;
	//TextureManager* texture;
	
	ImGuiManager* imgui;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;

	ModelLoader* object;
	ModelData plane;
	ModelData cube;

	ICamera* camera;
	ICamera* uiCamera;

	Input* input;

	Triangle* triangle;
	Triangle* triangle2;
	Sprite* sprite;
	Sprite* sprite2;
	Sphere* sphere;
	Model* model;
	Model* model2;

};

