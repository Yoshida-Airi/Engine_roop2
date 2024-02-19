#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<assert.h>
#include<array>
#include <iostream>
#include <filesystem>


#include"VectorMath.h"
#include"Globals.h"
#include"ModelData.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

class ModelLoader
{
public:

	static ModelLoader* GetInstance();


	~ModelLoader();
	/*void Initialize();
	void Update();
	void Draw();*/

	/// <summary>
	/// Objファイルを読むための関数
	/// </summary>
	ModelData LoadModelFile(const std::string& filename);


private:

	static ModelLoader* instance;

	static const size_t kMaxModel = 256;	//最大モデル数
	std::array<ModelData, kMaxModel> model;
	bool IsusedModel[kMaxModel] = { false };


private:

	Node ReadNode(aiNode* node);

};

