/**
*	@file ModelLoader.h
*	@brief モデル読み込みクラスヘッダ
*/


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

#include<map>
#include<wrl.h>


/**
*   @class ModelLoader
*	@brief  モデル読み込みクラス
*/
class ModelLoader
{
public:
	/// @brief シングルトンインスタンスを取得
	/// @return モデルローダーのインスタンス
	static ModelLoader* GetInstance();



	~ModelLoader();///< デストラクタ

	/// @brief OBJファイルを読み込む
	/// @param filename 読み込むモデルファイルのパス
	/// @return 読み込んだモデルデータ
	ModelData LoadModelFile(const std::string& filename);



private:

	static ModelLoader* instance;

	static const size_t kMaxModel = 256;	//最大モデル数
	std::array<ModelData, kMaxModel> model;
	bool IsusedModel[kMaxModel] = { false };


private:

	/// @brief ノードを読み込む
	/// @param node 読み込むノード
	/// @return 読み込んだノードデータ
	Node ReadNode(aiNode* node);


};

