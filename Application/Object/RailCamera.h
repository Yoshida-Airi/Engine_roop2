#pragma once
#include"ICamera.h"
#include"Camera.h"
#include"Model.h"
class RailCamera
{
public:
	~RailCamera();
	void Initialize();
	void Update();

	Vector3 GetWorldPosition();
	ICamera* camera = nullptr;
private:
	std::unique_ptr<Model> cameraModel = nullptr;

	
};

