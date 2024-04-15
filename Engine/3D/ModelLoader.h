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



//キーフレームを表現
template<typename tValue>
struct Keyframe
{
	float time;
	tValue value;
};

using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuatanion = Keyframe<Quaternion>;

template<typename tValue>
struct AnimationCurve
{
	std::vector<Keyframe<tValue>>Keyframes;
};

struct NodeAnimation
{
	AnimationCurve<Vector3>translate;
	AnimationCurve<Quaternion>rotate;
	AnimationCurve<Vector3>scale;
};


struct Animation
{
	float duration;	//アニメーション全体の尺（秒
	//NodeAnimationの集合。Node名でひけるように
	std::map<std::string, NodeAnimation>nodeAnimations;
};



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

	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);




private:

	static ModelLoader* instance;

	static const size_t kMaxModel = 256;	//最大モデル数
	std::array<ModelData, kMaxModel> model;
	bool IsusedModel[kMaxModel] = { false };


private:

	Node ReadNode(aiNode* node);


};

