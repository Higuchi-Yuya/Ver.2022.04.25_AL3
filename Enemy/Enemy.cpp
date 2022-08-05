#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Model* model) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//初期座標の設定
	worldTransform_.translation_ = {20, 3, 30};

}

void Enemy::Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	//フェーズ処理
	switch (phase_) { 
	case Phase::Approach:
	default:
		Approach_Update();
		break;

	case Phase::Laeve:
		Laeve_Update();
		break;
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach_Initialize() 
{

}


void Enemy::Approach_Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};
	//移動(ベクトルを加算)
	move = {0, 0, -enemy_speed_z};
	worldTransform_.translation_.z += move.z;

	//規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < -5.0f) {
		phase_ = Phase::Laeve;
	}

	//発射タイマーをデクリメント
	fireTimer_--;

	//規定時間に到達したら
	if (fireTimer_ <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Laeve_Update() 
{
	//敵移動ベクトル
	Vector3 move = {0, 0, 0};
	//移動(ベクトルを加算)
	move = {-enemy_speed_x, +enemy_speed_y, 0};
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
}

void Enemy::Fire() 
{
	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.1f;
	
	Vector3 enemy_pos = GetWorldPosition();
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity = player_pos - enemy_pos;
	velocity.normalize();
	velocity *= kBulletSpeed;

	////速度ベクトルを自機の向きに合わせて回転させる
	//velocity = velocity * worldTransform_.matWorld_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

Vector3 Enemy::GetWorldPosition() 
{ 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}



void Enemy::OnCollision() {}

