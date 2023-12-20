#pragma once
#include"Framework.h"

#include"IScene.h"

/// <summary>
/// ゲーム固有のクラス
/// </summary>
class MyGame :public Framework
{
public:
	~MyGame();
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:


};

