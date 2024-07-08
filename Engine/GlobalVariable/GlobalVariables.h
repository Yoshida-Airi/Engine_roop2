#pragma once
#include<variant>
#include<map>
#include<string>

#include"VectorMath.h"

class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	struct Item
	{
		std::variant<int32_t, float, Vector3>value;
	};

	struct Group
	{
		std::map<std::string, Item>items;
	};

	void Update();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	//値のセット
	//int
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//float
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//Vector3
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;

private:
	std::map<std::string, Group>datas_;


	//静的メンバ変数の宣言と初期化
	static GlobalVariables* instance;
};
