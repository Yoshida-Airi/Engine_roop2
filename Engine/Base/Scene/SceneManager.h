#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:

	//シングルトン
	static SceneManager* GetInstance();
	~SceneManager();
	void Update();
	void Draw();

	/// <summary>
	/// 次シーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory);

private:
	static SceneManager* instance;	//シングルトン
	AbstractSceneFactory* sceneFactory_ = nullptr;
	//今のシーン
	BaseScene* scene_ = nullptr;
	BaseScene* nextSscene_ = nullptr;
};

