#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"TextureManager.h"
#include"Title.h"
#include"Game.h"
#include"SceneManager.h"

class Framework
{
public:
	virtual ~Framework();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	void PostDraw();
	void Run();

protected:
	WinApp* winApp;
	DirectXCommon* dxCommon;
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;
	
};

