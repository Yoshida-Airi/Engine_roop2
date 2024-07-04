#pragma once

#include"Model.h"
#include"Collider.h"

class GameObject
{
protected:
	std::vector<Model*>models_;

public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(Camera* camera);

	

};

