#include "MapChipField.h"
#include<fstream>
#include<sstream>

void MapChipField::ResetMapChipData()
{
	//マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical_);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data)
	{
		mapChipDataLine.resize(kNumBlockHorizontal_);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath)
{
	//マップチップデータをリセット
	ResetMapChipData();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップCSV
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();

	//CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical_; ++i)
	{
		std::string line;
		getline(mapChipCsv, line);

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal_; ++j)
		{
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word))
			{
				mapChipData_.data[i][j] = mapChipTable[word];
				
			}

		}

	}

}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex)
{
	if (xIndex < 0 || kNumBlockHorizontal_ - 1 < xIndex)
	{
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical_ - 1 < yIndex)
	{
		return MapChipType::kBlock;
	}

	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex)
{
	return Vector3(kBlockWidth_ * xIndex, kBlockHeight_ * (kNumBlockVirtical_ - 1 - yIndex), 0);
}

uint32_t MapChipField::GetNumBlockVertical()
{ 
	return kNumBlockVirtical_;
}

uint32_t MapChipField::GetNumBlockHorizontal()
{ 
	return kNumBlockHorizontal_; 
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position)
{
	IndexSet indexset = {};
	indexset.xIndex = static_cast<uint32_t>((position.x + kBlockWidth_ / 2) / kBlockWidth_);
	float y = (position.y + kBlockHeight_ / 2) / kBlockHeight_;
	indexset.yIndex = static_cast<uint32_t>(kNumBlockVirtical_ - 1 - y);
	return indexset;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex)
{
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth_ / 2.0f;
	rect.right = center.x + kBlockWidth_ / 2.0f;
	rect.bottom = center.y + kBlockHeight_ / 2.0f;
	rect.top = center.y - kBlockHeight_ / 2.0f;

	return rect;

}
