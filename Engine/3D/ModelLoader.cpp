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

ModelData ModelLoader::LoadModelFile(const std::string& filename)
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

	model.at(index).rootNode = ReadNode(scene->mRootNode);

	return model[index];
}

Node ModelLoader::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	result.localMatrix.m[0][3] = aiLocalMatrix[0][3];

	result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	result.localMatrix.m[1][3] = aiLocalMatrix[1][3];

	result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[2][3];

	result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	result.name = node->mName.C_Str();//Node名を格納
	result.children.resize(node->mNumChildren);	//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}



ModelLoader* ModelLoader::instance = NULL;