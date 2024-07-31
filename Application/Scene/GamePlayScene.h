#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ParticleSystem.h"
#include"Camera.h"

#include"CollisionManager.h"

#include"Input.h"

#include"Object/Player/Player.h"
#include"Object/Enemy/Enemy.h"
#include"Object/Skydome/Skydome.h"
#include"Object/CameraController/CameraController.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"
#include"Object/Goal/Goal.h"

#include"LevelEditor.h"


/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene:public BaseScene
{
public:
	~GamePlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void CheckAllCollisions();

	void SpawnEnemy(const Vector3& position);
	void SpawnBlock(const Vector3& position, const Vector3& scale);

private:
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;

	//当たり判定処理
	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;


	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;
	uint32_t circle;

	uint32_t configTexture;
	

	Camera* camera;
	

	Input* input;


	std::unique_ptr<Triangle> triangle = nullptr;
	std::unique_ptr<Triangle> triangle2 = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Sphere> sphere = nullptr;
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> model2 = nullptr;

	std::unique_ptr<ParticleSystem> particle = nullptr;
	std::unique_ptr<ParticleSystem> particle2 = nullptr;

	LevelEditor* levelEditor = nullptr;

	std::unique_ptr <Player> player;
	std::list<Enemy*> enemys;
	std::unique_ptr <Skydome>skydome;
	std::unique_ptr <Goal>goal;
	CameraController* cameraController;
	std::unique_ptr<Weapon>weapon;
	std::list<Ground*>grounds;

	std::unique_ptr<Sprite> config = nullptr;
};

