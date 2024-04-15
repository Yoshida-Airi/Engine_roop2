#pragma once
#include"BaseScene.h"

#include"Model.h"
#include"Camera.h"

class TestPlayScene:public BaseScene
{
public:
	~TestPlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:
	Camera* camera;

	std::unique_ptr<Model> model = nullptr;

};

