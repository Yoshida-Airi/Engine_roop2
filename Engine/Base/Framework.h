#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"TextureManager.h"
#include"Input.h"
#include"SceneManager.h"
#include"ImGuiManager.h"
#include"AbstractSceneFactory.h"

class Framework
{

public:

	~Framework();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();

protected:
	//WindowAPIの初期化
	WinApp* winApp = nullptr;
	//DirectXの初期化
	DirectXCommon* dxCommon = nullptr;
	TextureManager* texture = nullptr;
	Input* input;
	//IScene* scene_ = nullptr;	//シーン
	SceneManager* sceneManager_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;


	ImGuiManager* imgui;



};

