#pragma once
#include"BaseScene.h"

#include"Animation.h"
#include"Model.h"
#include"Camera.h"

#include"Input.h"

class TestPlayScene:public BaseScene
{
public:
	~TestPlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	//Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);


private:
	Camera* camera;
	
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> model2 = nullptr;
	std::unique_ptr<Model> model3 = nullptr;
	std::unique_ptr<Model> model4 = nullptr;

	AnimationData walkAnimation;

	void Move();

	float Lerp(const float& a, const float& b, float t);

	float LerpShortAngle(float a, float b, float t);

	float angle;
	float speed = 0.05f;
	bool isMoving = false;
};

