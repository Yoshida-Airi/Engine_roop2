#pragma once
#include"ImGuiManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"


class Skydome
{
public:

	~Skydome();
	void Initialize(ICamera* camera, ModelData skydome, const Vector3& position);
	void Update();
	void Draw();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* skydome_ = nullptr;

	ICamera* camera_;
};

