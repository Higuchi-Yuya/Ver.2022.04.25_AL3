#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <math.h>
#include <memory>
#include <list>

#include "Affine_trans.h"
#include "PlayerBullet.h"
class Player {

  public:
	~Player();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	void Attack();

	//描画
	void Draw(ViewProjection viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetWorldTransformParent(WorldTransform *worldtransform);
  private:
	//移動処理
	void Move();

	void Rotation();

	//行列変換
	void Trans_Update();

	//デバッグテキスト
	void Debug_Text();

//メンバ変数
  private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform cameraworld_;
	//モデル
	Model* model_ = nullptr;

	//プレイヤー関連
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//スピード
	float character_speed_x = 0.2f;
	float character_speed_y = 0.2f;

	float rotation_speed_y = 0.01f;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
