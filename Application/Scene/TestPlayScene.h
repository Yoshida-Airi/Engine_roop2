/**
*	@file TestPlayScene.h
*	@brief テストシーンクラスヘッダ
*/

#pragma once
#include"BaseScene.h"

#include"Animation.h"
#include"Model.h"
#include"Camera.h"

#include"Input.h"

#include"LevelEditor.h"

#include<vector>

#include"Object/Ground/MapChipField.h"

/**
*   @class TestPlayScene
*	@brief  テストシーンクラス
*/
class TestPlayScene:public BaseScene
{
public:
	~TestPlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	//Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);


private:
	Camera* camera;
	
	std::unique_ptr<Model> walk = nullptr;
	std::unique_ptr<Model> model2 = nullptr;
	std::unique_ptr<Model> model3 = nullptr;
	std::unique_ptr<Model> model4 = nullptr;

	

	AnimationData walkAnimation;

	LevelEditor* levelEditor;


	std::vector<std::vector<Model*>>blocks_;

	MapChipField* mapChipField_;

	void GenerateBlocks();

};

