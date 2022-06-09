#include "Enemy.h"

void Enemy::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//初期座標の設定
	worldTransform_.translation_ = {0, 3, 30};
}
void Enemy::Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};
	move = {0, 0, -enemy_speed_z};
	worldTransform_.translation_.z += move.z;

	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
