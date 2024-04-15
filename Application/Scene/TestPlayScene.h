#pragma once
#include"BaseScene.h"

#include"Model.h"

class TestPlayScene:public BaseScene
{
public:
	~TestPlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;


private:
	std::unique_ptr<Model> model = nullptr;

};

