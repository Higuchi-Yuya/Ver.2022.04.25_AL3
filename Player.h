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
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	void Attack();

	//描画
	void Draw(ViewProjection viewProjection);

  private:

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//プレイヤー関連
	//アフィン
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//スピード
	float character_speed_x = 0.2f;
	float character_speed_y = 0.2f;

	float rotation_speed_y = 0.2f;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
