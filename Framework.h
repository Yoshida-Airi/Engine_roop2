#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"TextureManager.h"
#include"Input.h"

class Framework
{

public:

	~Framework();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw() = 0;

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

};

