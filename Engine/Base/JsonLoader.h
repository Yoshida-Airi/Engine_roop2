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

	};
	std::vector<ObjectData>objects;
};

class JsonLoader
{
public:

	~JsonLoader();
	void LoaderJsonFile();

	void Update();
	void Draw(Camera* camera);

private:

	std::map<std::string, std::unique_ptr<Model>> models;
	std::unique_ptr<LevelData> levelData;
};

