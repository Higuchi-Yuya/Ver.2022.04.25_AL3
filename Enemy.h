#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class Enemy 
{
  public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	float enemy_speed_z = 0.1;

	//アフィン用
	Affine_trans* trans = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};
