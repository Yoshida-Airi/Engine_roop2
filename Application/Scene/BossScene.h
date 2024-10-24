/**
*	@file BossScene.h
*	@brief ボスシーンクラスヘッダ
*/

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
#include"Object/Enemy/FlyEnemy.h"
#include"Object/Skydome/Skydome.h"              
#include"Object/CameraController/CameraController.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"
#include"Object/Goal/Goal.h"
#include"Effect/DeathEffect.h"
#include"Effect/Fade.h"
#include"Object/Enemy/Boss.h"

#include"LevelEditor.h"


/**
*   @class BossScene
*	@brief  ボスシーンクラス
*/
class BossScene :public BaseScene
{
public:
	~BossScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void CheckAllCollisions();

	void SpawnBlock(const Vector3& position, const Vector3& scale);

	void CreateDeathEffect(Vector3 position);

	enum class Phase
	{
		kPlay,
		kClear,
		kDeath,
	};


	void ChangePhase(Phase phase);


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
	uint32_t HPTexture;


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

	std::unique_ptr<Fade> fade_ = nullptr;

	LevelEditor* levelEditor = nullptr;

	std::unique_ptr <Player> player;
	std::list<Enemy*> enemys;
	std::list<FlyEnemy*> flyEnemys;
	std::unique_ptr<Boss> boss;
	std::unique_ptr <Skydome>skydome;
	std::unique_ptr <Goal>goal;
	CameraController* cameraController;
	std::unique_ptr<Weapon>weapon;
	std::list<Ground*>grounds;
	std::list<DeathEffect*>deathEffect_;	//敵消滅エフェクト
	//　std::unique_ptr<DeathEffect> playerDeathEffect = nullptr;

	std::unique_ptr<Sprite> config = nullptr;
	std::unique_ptr<Sprite> hp1 = nullptr;
	std::unique_ptr<Sprite> hp2 = nullptr;
	std::unique_ptr<Sprite> hp3 = nullptr;
	std::unique_ptr<Sprite> hp4 = nullptr;
	std::unique_ptr<Sprite> hp5 = nullptr;

	Phase phase_;

private:

	void GamePlayPhase();
	void GameClearPhase();
	void GameOverPhase();
};

