#pragma once
#include"BaseScene.h"

#include"Animation.h"
#include"Model.h"
#include"Camera.h"

#include"Input.h"

#include"LevelEditor.h"

#include"Light.h"

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

	LevelEditor* levelEditor;

	PointLight* pointLight;

};

