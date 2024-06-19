#include "JsonLoader.h"


JsonLoader::~JsonLoader()
{
	delete levelData;

	// `WorldTransform` オブジェクトを解放
	for (auto& object : objects) 
	{
		delete object;
	}

	models.clear();
	objects.clear();

}

void JsonLoader::LoaderJsonFile()
{
	const std::string fullpath = "Resources/levelEditor.json";
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get < std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.filename = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];

			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];

			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			//コライダーのパラメータ読み込み
			nlohmann::json& collider = object["collider"];



		}

		//オブジェクト走査を再帰関数にまとめ、再起呼出で枝を走査する
		if (object.contains("children"))
		{

		}

	}

}

void JsonLoader::Update()
{


	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		model.reset(Model::Create("Resources/SampleAssets/cube.obj"));

		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) { model.reset(it->second); }
		//モデルを指定して3Dオブジェクトを生成
		WorldTransform* newObject = new WorldTransform();
		newObject->translation_ = objectData.translation;
		newObject->rotation_ = objectData.rotation;
		newObject->scale_ = objectData.scaling;
		newObject->Initialize();
		//配列に登録
		objects.push_back(newObject);
	}


	for (WorldTransform* object : objects)
	{
		object->TransferMatrix();

		model->GetWorldTransform()->translation_ = object->translation_;
		model->GetWorldTransform()->rotation_ = object->rotation_;
		model->GetWorldTransform()->scale_ = object->scale_;

	}


}

void JsonLoader::Draw(Camera* camera)
{
	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) { model.reset(it->second); }


		model->Draw(camera);

	}
}
