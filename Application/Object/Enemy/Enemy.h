#pragma once
#include"Model.h"

class Enemy
{
public:
	void Initialize(const ModelData enemyData, const ModelData bulletData);
	void Update();
	void Draw(ICamera* camera);

private:
	
	std::unique_ptr<Model>enemy = nullptr;
	ModelData enemyData_;

	////弾
	//std::list<EnemyBullet*> bullets_;
	ModelData bulletData_;


};

