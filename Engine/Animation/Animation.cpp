#include "Animation.h"

Animation* Animation::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Animation;
	}
	return instance;
}

//アニメーションファイル読み込み
AnimationData Animation::LoadAnimationFile(const std::string& filename)
{
	AnimationData animation;	//今回作るアニメーション

	Assimp::Importer importer;
	std::string filePath = filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

	if (scene->mNumAnimations == 0)//アニメーションがない
	{
		animation.isValid = false;
		return animation;
	}
	else
	{
		animation.isValid = true;
	}

	aiAnimation* animationAssimp = scene->mAnimations[0];	//最初のアニメーションだけ採用
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);	//時間の単位を病に変換

	//NodeAnimationを解析
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex)
	{
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		//translate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };	//右手から左手
			nodeAnimation.translate.Keyframes.push_back(keyframe);
		}


		//rotate
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex)
		{
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuatanion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
			nodeAnimation.rotate.Keyframes.push_back(keyframe);
		}

		//scale
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex)
		{
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);	//秒に変換
			keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };	//右手から左手
			nodeAnimation.scale.Keyframes.push_back(keyframe);
		}


	}

	return animation;
}


//3次元ベクトルの線形補完
Vector3 Animation::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	assert(!keyframes.empty());	//キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time)
	{
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index)
	{
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
		{
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.begin()).value;
}

//クォータニオンの線形補間
Quaternion Animation::CalculateValue(const std::vector<KeyframeQuatanion>& keyframes, float time)
{
	assert(!keyframes.empty());	//キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time)
	{
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index)
	{
		size_t nextIndex = index + 1;
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
		{
			//範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.begin()).value;
}

Animation* Animation::instance = NULL;