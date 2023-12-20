#pragma once

class SceneManager;

class BaseScene
{
public:

	BaseScene(SceneManager* sceneManager);
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;


protected:

	//シーンマネージャー（借りてくる）
	SceneManager* sceneManager_ = nullptr;
};

