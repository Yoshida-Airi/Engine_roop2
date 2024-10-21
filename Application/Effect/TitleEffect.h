/**
*	@file TitleEffect.h
*	@brief タイトルエフェクトクラスヘッダ
*/

#pragma once
#include"ParticleSystem.h"

/**
* @class TitleEffect
* @brief タイトル画面のパーティクルエフェクトを制御するクラス
* @details パーティクルエフェクトの初期化、更新、描画を行い、タイトル画面で表示する演出を管理する
*/
class TitleEffect
{
public:

	/**
	* @brief タイトルエフェクトの初期化処理
	* @param camera カメラオブジェクト
	* @details パーティクルエフェクトの位置や動作を、カメラ情報を基に初期化する
	*/
	void Initialize(Camera* camera);

	/**
	* @brief タイトルエフェクトの更新処理
	* @details 毎フレーム実行され、パーティクルエフェクトの進行を管理する
	*/
	void Update();

	/**
	* @brief タイトルエフェクトの描画処理
	* @details パーティクルシステムを使用して、タイトルエフェクトを画面に描画する
	*/
	void Draw();

	/**
	* @brief エフェクトの実行状態を設定する
	* @param isFlag エフェクトを実行するかどうかのフラグ（trueなら実行、falseなら非実行）
	* @details このフラグを使用して、エフェクトの実行・停止を制御する
	*/
	void SetFlag(bool isFlag)
	{
		flag = isFlag;
	}

	/**
	* @brief エフェクトの表示位置を設定する
	* @param position エフェクトのエミッター位置
	* @details パーティクルエフェクトのエミッター位置を設定する
	*/
	void SetPosition(Vector3 position)
	{
		hitEffect->emitter_->transform.translate.x = position.x;
		hitEffect->emitter_->transform.translate.y = position.y + 0.7f;
		hitEffect->emitter_->transform.translate.z = position.z;
	}

	/**
	* @brief エフェクトが終了（消滅）したかどうかを判定する
	* @return エフェクトが終了した場合はtrue、まだ表示中ならfalse
	* @details パーティクルエフェクトの表示時間が終了したかを確認する
	*/
	bool IsDead();

private:

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<ParticleSystem> hitEffect = nullptr;
	uint32_t circleTexture;

	bool flag;	//実行中かどうかのフラグ
	int playTime = 20;
	bool isDead = false;


};
