#pragma once
class IScene
{
public:
	virtual ~IScene() = default;
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
};

