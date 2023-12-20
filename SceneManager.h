#pragma once
#include"BaseScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:

	~SceneManager();
	void Update();
	void Draw();

	/// <summary>
	///次シーン予約
	/// </summary>
	/// <param name="nextScene">次シーン</param>
	void IsNextScene(BaseScene* nextScene) {
		nextSscene_ = nextScene;
	}

private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	BaseScene* nextSscene_ = nullptr;
};

