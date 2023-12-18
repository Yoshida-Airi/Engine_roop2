#pragma once
class SceneManager;
class IScene
{
public:
	IScene(SceneManager* sceneManager);
	virtual ~IScene() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void SetSceneManager(SceneManager* sceneManager);
protected:
	SceneManager* sceneManager_ = nullptr;
};

