#pragma once
#include"Camera.h"
#include"Sprite.h"

class Fade
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	enum class Status
	{
		None,
		FadeIn,
		FadeOut,
	};

	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name="status"></param>
	/// <param name="duration"></param>
	void Start(Status status, float duration);

	/// <summary>
	/// フェード終了
	/// </summary>
	void Stop();

	/// <summary>
	/// フェード終了判定
	/// </summary>
	/// <returns></returns>
	bool IsFinished()const;

private:

	uint32_t fadeTex;
	std::unique_ptr< Sprite> sprite_ = nullptr;

	Status status_ = Status::None;

	float duration_ = 0.0f;		//フェードの持続時間
	float counter_ = 0.0f;		//経過時間カウンター


};

