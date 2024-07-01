#include "LevelEditor.h"


LevelEditor::~LevelEditor()
{


}

void LevelEditor::LoaderJsonFile()
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
	levelData.reset(new LevelData());

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

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
	
			std::string type = collider["type"].get<std::string>();
			objectData.collisionType = type;

			objectData.center.x = (float)collider["center"][0];
			objectData.center.y = (float)collider["center"][1];
			objectData.center.z = (float)collider["center"][2];

			objectData.size.x = (float)collider["size"][0];
			objectData.size.y = (float)collider["size"][1];
			objectData.size.z = (float)collider["size"][2];

		}

		//オブジェクト走査を再帰関数にまとめ、再起呼出で枝を走査する
		if (object.contains("children"))
		{

		}

	}

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it == models.end())
		{
			//Model* model = Model::Create(objectData.filename);
			models[objectData.filename].reset(Model::Create(objectData.filename));
		
		}
	}

	for (auto& objectData : levelData->objects)
	{
		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<WorldTransform> newObject = std::make_unique<WorldTransform>();
		newObject->Initialize();
		newObject->translation_ = objectData.translation;
		newObject->rotation_ = objectData.rotation;
		newObject->scale_ = objectData.scaling;
		newObject->UpdateWorldMatrix();
		//配列に登録
		objects.push_back(std::move(newObject));
		
	}



}

void LevelEditor::Update()
{

	


}

void LevelEditor::Draw(Camera* camera)
{
	int i = 0;
	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects)
	{
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end())
		{
			model = (it->second.get());
		}
		if (model)
		{
			//model->SetWorldTransform(objects[i].get());
			model->GetWorldTransform()->constMap = (objects[i].get()->constMap);
			model->GetWorldTransform()->constBuffer_ = (objects[i].get()->constBuffer_);
			model->GetWorldTransform()->matWorld_ = objects[i].get()->matWorld_;
			model->GetWorldTransform()->parent_ = objects[i].get()->parent_;
			model->GetWorldTransform()->translation_ = (objects[i].get()->translation_);
			model->GetWorldTransform()->rotation_ = (objects[i].get()->rotation_);
			model->GetWorldTransform()->scale_ = (objects[i].get()->scale_);
			//model->GetWorldTransform()->UpdateWorldMatrix();
			model->Update();

			model->Draw(camera);

			// iを含むデバッグ名を作成
			std::ostringstream debugName;
			debugName << "mapData _" << i;

			model->ModelDebug(debugName.str().c_str());

		}

		i++;

	}
}
