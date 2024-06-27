#pragma once
#include<list>
#include"Collider.h"
#include"MathUtilty.h"
#include"Model.h"

class CollisionManager {

public:

	void Initialize();
	void UpdateWorldTransform();
	void Draw(Camera* camera);

	/// <summary>
	/// リストをクリアする
	/// </summary>
	void ListClear();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void ChackAllCollisions();

	/// <summary>
	/// コライダーをリストに登録する
	/// </summary>
	/// <param name="collider">オブジェクト</param>
	void AddColliders(Collider* collider);


private:
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheakCollisionPair(Collider* colliderA, Collider* colliderB);

	// コライダーリスト
	std::list<Collider*> colliders_;

	std::unique_ptr<Model>model;

};