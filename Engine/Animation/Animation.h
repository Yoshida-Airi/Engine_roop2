#pragma once
#include <string>
#include<map>
#include"VectorMath.h"
#include"MathUtilty.h"
#include<vector>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

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


struct AnimationData
{
	float duration;	//アニメーション全体の尺（秒
	//NodeAnimationの集合。Node名でひけるように
	std::map<std::string, NodeAnimation>nodeAnimations;
};


class Animation
{
public:
	static Animation* GetInstance();


	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuatanion>& keyframes, float time);

private:
	static Animation* instance;

};

