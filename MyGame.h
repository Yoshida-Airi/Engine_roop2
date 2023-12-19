#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"ImGuiManager.h"
#include"TextureManager.h"
#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ICamera.h"
#include"Input.h"
#include"Camera.h"
#include"UICamera.h"

//ゲーム固有のクラス
class MyGame
{
public:
	~MyGame();
	void Initialize();
	void Update();
	void Draw();

	bool IsEndReqest() { return endReqest_; };

private:

	//WindowAPIの初期化
	WinApp* winApp = nullptr;
	//DirectXの初期化
	DirectXCommon* dxCommon = nullptr;
	TextureManager* texture = nullptr;
	Input* input;

	bool endReqest_ = false;


	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;

	ModelLoader* object;
	ModelData plane;
	ModelData cube;

	ICamera* camera;
	ICamera* uiCamera;


#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

	Triangle* triangle = nullptr;
	Triangle* triangle2 = nullptr;
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;
	Sphere* sphere = nullptr;
	Model* model = nullptr;
	Model* model2 = nullptr;

};

