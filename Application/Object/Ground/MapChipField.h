#pragma once
#include <stdint.h>
#include<vector>
#include<string>
#include<map>

#include<cassert>

#include<MathUtilty.h>

enum class MapChipType
{
	kBlank,
	kBlock,
};

struct MapChipData
{
	std::vector<std::vector<MapChipType>>data;
};

namespace
{
	std::map<std::string, MapChipType>mapChipTable =
	{
		{"0",MapChipType::kBlank},
		{"1",MapChipType::kBlock},
	};
}



class MapChipField
{
public:


	struct IndexSet
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect
	{
		float left;
		float right;
		float bottom;
		float top;
	};

	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ種別の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標の取得
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVertical();
	uint32_t GetNumBlockHorizontal();

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

private:
	void ResetMapChipData();

};

