#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class EnemyBullet 
{
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 4;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	//アフィン用
	Affine_trans* trans = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};