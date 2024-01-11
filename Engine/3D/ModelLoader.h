#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<assert.h>
#include<array>

#include"VectorMath.h"
#include"Globals.h"
#include"ModelData.h"

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
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);


private:

	static ModelLoader* instance;

	static const size_t kMaxModel = 256;	//最大モデル数
	std::array<ModelData, kMaxModel> model;
	bool IsusedModel[kMaxModel] = { false };


private:


	/// <summary>
	/// mtlファイルを読むための関数
	/// </summary>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
};

