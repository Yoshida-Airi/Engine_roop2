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

ModelData ModelLoader::LoadObjFile(const std::string& filename)
{

	uint32_t index = 0;
	const std::string& filePathName = "Resources/" + filename;


	std::filesystem::path path(filePathName);
	std::string filename1 = path.parent_path().string();

	std::filesystem::path path2(filename);
	std::string filename2 = path2.parent_path().string();

	for (int i = 0; i < kMaxModel; i++)
	{
		//同じモデルがあった場合
		if (model[i].filename == filePathName)
		{
			return model[i];
		}

		if (IsusedModel[i] == false) {
			index = i;
			IsusedModel[i] = true;
			model.at(index).filename = filePathName;
			break;
		}
	}

	//indexが不正な値だった場合止める
	if (index < 0 || kMaxModel <= index) {
		assert(false);
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePathName.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	//meshの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		//faceの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			//vertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				//modelData.vertices.push_back(vertex);
				model.at(index).vertices.push_back(vertex);
			}
		}
	}

	//テクスチャの解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			model.at(index).material.textureFilePath = filename2 + "/" + textureFilePath.C_Str();
		}
	}

	return model[index];
}



ModelLoader* ModelLoader::instance = NULL;