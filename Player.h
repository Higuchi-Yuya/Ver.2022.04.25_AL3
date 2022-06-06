#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <math.h>

#include "Affine_trans.h"
class Player {
  public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

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
	
	//スピード
	float character_speed_x = 0.2f;
	float character_speed_y = 0.2f;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};
