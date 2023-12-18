#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"ImGuiManager.h"
#include"TextureManager.h"

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
	
};

