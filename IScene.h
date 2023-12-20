#pragma once
class IScene
{
public:

public:
	virtual ~IScene() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;


private:

};

