#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"TextureManager.h"
#include"IScene.h"
#include"Title.h"
#include"Game.h"


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
	IScene* scene;
	
};

