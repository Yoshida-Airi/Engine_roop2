#include "ModelLoader.h"

ModelLoader* ModelLoader::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ModelLoader;
	}
	return instance;
}

ModelLoader::~ModelLoader()
{
}

ModelData ModelLoader::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	uint32_t index = 0;

	for (int i = 0; i < kMaxModel; i++)
	{
		//同じモデルがあった場合
		if (model[i].filename == directoryPath + "/" + filename)
		{
			return model[i];
		}

		if (IsusedModel[i] == false) {
			index = i;
			IsusedModel[i] = true;
			model.at(index).filename = directoryPath + "/" + filename;
			break;
		}
	}

	//indexが不正な値だった場合止める
	if (index < 0 || kMaxModel <= index) {
		assert(false);
	}


	//1.中で必要となる変数の宣言
	std::vector<Vector4>positions_;	//位置
	std::vector<Vector3>normals_;	//法線
	std::vector<Vector2>texcoords_;	//テクスチャ座標
	std::string line;	//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);	//ファイルを開く
	assert(file.is_open());	//とりあえず開けなかったら止める

	//3.実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;	//先頭の識別子を読む

		//identifierに応じた処理
		if (identifier == "v")
		{
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.z *= -1.0f;
			position.w = 1.0f;
			positions_.push_back(position);
		}
		else if (identifier == "vt")
		{
			Vector2 texccoord;
			s >> texccoord.x >> texccoord.y;
			texccoord.y = 1.0f - texccoord.y;
			texcoords_.push_back(texccoord);
		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals_.push_back(normal);
		}
		else if (identifier == "f")
		{
			VertexData triangle[3];
			//面は三角形限定。その他は未対応。
			for (int32_t faveVertex = 0; faveVertex < 3; ++faveVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/');	//区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}

				//要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions_[elementIndices[0] - 1];
				Vector2 texcoord = texcoords_[elementIndices[1] - 1];
				Vector3 normal = normals_[elementIndices[2] - 1];

				triangle[faveVertex] = { position,texcoord,normal };
			}

			//頂点を逆順で登録することで、周り順を逆にする
			model.at(index).vertices.push_back(triangle[2]);
			model.at(index).vertices.push_back(triangle[1]);
			model.at(index).vertices.push_back(triangle[0]);
		}

		else if (identifier == "mtllib")
		{
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にObjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			model.at(index).material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}



	//4.ModelDataを返す
	return model[index];
}


MaterialData ModelLoader::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	//1.中で必要となる変数の宣言
	MaterialData materialData;	//構築するMaterialData
	std::string line;	//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());//とりあえず開けなかったら止める

	//3.実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	//4.MaterialDataを返す
	return materialData;
}


ModelLoader* ModelLoader::instance = NULL;