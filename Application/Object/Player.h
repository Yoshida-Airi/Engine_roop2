#pragma once
#include"Model.h"
#include"Input.h"

class Player
{
public:
	void Initialize(ModelData playerData);
	void Update();
	void Draw(ICamera* camera);

private://メンバ変数
	Input* input_ = nullptr;


	std::unique_ptr<Model> player = nullptr;

private://メンバ関数

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転（旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

};

