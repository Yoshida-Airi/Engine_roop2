#pragma once
#include <string>
#include<map>
#include<array>
#include"VectorMath.h"
#include"MathUtilty.h"
#include<vector>
#include<optional>
#include"Transform.h"
#include"ModelData.h"

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
	bool isValid;	//アニメーションデータが存在しているかどうか
	float duration;	//アニメーション全体の尺（秒
	std::string filename{};
	//NodeAnimationの集合。Node名でひけるように
	std::map<std::string, NodeAnimation>nodeAnimations;

};

struct Joint
{
	QuaternionTransform transform; //Transform情報
	Matrix4x4 localMatrix;
	Matrix4x4 sleletonSpaceMatrix;	//sleltonSpaceでの変換行列
	std::string name;	//名前
	std::vector<int32_t>children;	//子JointのIndexのリスト。いなければ空
	int32_t index;	//自身のIndex
	std::optional<int32_t>parent;	//親JointのIndex。いなければnull
};

struct Skeleton
{
	int32_t root;	//RootjointのIndex
	std::map < std::string, int32_t>jointmap;	//Joint名とIndexとの辞書
	std::vector<Joint>joints;	//所属しているジョイント
};

class Animation
{
public:
	static Animation* GetInstance();

	void Update(Skeleton& skelton);

	AnimationData LoadAnimationFile(const std::string& filename);

	Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyframeQuatanion>& keyframes, float time);

	Skeleton CreateSkelton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joint);
	
	/// <summary>
	/// スケルトンに対してアニメーションの適用をする
	/// </summary>
	/// <param name="skelton">スケルトン情報（骨）</param>
	/// <param name="animationData">適用するアニメーションデータ</param>
	/// <param name="animationTime">アニメーションの時間</param>
	void ApplyAnimation(Skeleton& skelton, const AnimationData& animationData, float animationTime);

private:

	static const size_t kMaxAnimation = 256;	//最大モデル数
	std::array<AnimationData, kMaxAnimation> animationDatas;
	bool isUsedAnimation[kMaxAnimation] = { false };

	static Animation* instance;

};

