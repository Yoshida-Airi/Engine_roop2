/**
*	@file MyGame.h
*	@brief ゲームクラスヘッダ
*/

#pragma once
#include"Framework.h"

#include"BaseScene.h"

/**
*   @class MyGame
*	@brief  固有クラス
*/
class MyGame :public Framework
{
public:
	~MyGame();
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	BaseScene* scene;

};

