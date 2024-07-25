#pragma once
#include"json.hpp"
#include<fstream>
#include<assert.h>
#include"VectorMath.h"
#include"Model.h"
#include<vector>
#include"WorldTransform.h"
#include"Camera.h"

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

		// colliderの情報
		std::string collisionType;	//コライダーのタイプ
		Vector3 center;  // コライダーの中心
		Vector3 size;    // コライダーのサイズ

	};
	std::vector<ObjectData>objects;
};

class LevelEditor
{
public:

	~LevelEditor();
	void LoaderJsonFile();

	void Update();
	void Draw(Camera* camera);

	void objectTraversal(nlohmann::json& object);

private:

	std::map<std::string, std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<WorldTransform>> objects;
	std::unique_ptr<LevelData> levelData;
};

