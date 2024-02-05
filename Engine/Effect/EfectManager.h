#pragma once
#include<wrl.h>
#include"Sprite.h"

struct FadeConstants
{
	float fadeInProgress;
};

class EfectManager
{
	
public:
	void Initialize();
	void Update(float deltaTime);
	void Draw(Camera* camera);

	
private:
	TextureManager* texture;
	Camera* camera_;

	uint32_t blackTexture;

	std::unique_ptr<Sprite> fadeIn = nullptr;

	float deltaTime_ = 0.0f;

	bool initialized_ = false;


};

