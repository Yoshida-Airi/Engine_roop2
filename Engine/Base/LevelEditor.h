#pragma once
#include"json.hpp"
#include<fstream>
#include<assert.h>
#include"VectorMath.h"
#include"Model.h"
#include<vector>
#include"WorldTransform.h"
#include"Camera.h"
#include"Collider.h"

//レベルデータ
struct LevelData 
{
	//オブジェクト1個分のデータ
	struct ObjectData
	{
		std::string filename;	//ファイル名
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;

		std::string collisionType;
		Vector3 center;
		Vector3 size;

	};
	std::vector<ObjectData>objects;
};

class LevelEditor : public Collider
{
public:

	~LevelEditor();
	void LoaderJsonFile();

	void Update();
	void Draw(Camera* camera);

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:

	std::map<std::string, std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<WorldTransform>> objects;
	std::unique_ptr<LevelData> levelData;
};

