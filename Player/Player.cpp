#include "Player.h"
#include "RailCamera.h"

//RailCamera* railCamera = nullptr;

Player::~Player() 
{
	
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換を初期化
	worldTransform_.translation_.z = 50.0f;
	worldTransform_.Initialize();
	



	
}

void Player::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//回転処理
	Rotation();

	//移動処理
	Move();

	//キャラクターの攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	

	//行列の更新および転送
	Trans_Update();

	//デバッグテキスト
	Debug_Text();
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_Y)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		Affine_trans trans;

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = velocity * worldTransform_.matWorld_;

		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::SetWorldTransformParent(WorldTransform *worldtransform) 
{
	worldTransform_.parent_ = worldtransform;
}

void Player::Move() 
{
	Vector3 move = {0, 0, 0};
	constexpr float MoveLimitX = 35;
	constexpr float MoveLimitY = 19;

	//キャラクターの移動処理
	if (input_->PushKey(DIK_LEFT)) {
		move = {-character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_UP)) {
		move = {0, character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	}

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MoveLimitY);
}

void Player::Rotation() 
{ 
	Vector3 rotate = {0, 0, 0};

	//キャラクターの旋回移動
	if (input_->PushKey(DIK_U)) {
		rotate = {0, -rotation_speed_y, 0};
		worldTransform_.rotation_.y += rotate.y;
	}
	if (input_->PushKey(DIK_I)) {
		rotate = {0, +rotation_speed_y, 0};
		worldTransform_.rotation_.y += rotate.y;
	}
}

void Player::Trans_Update() 
{
	Affine_trans* trans = nullptr;
	
	//ベクトルの加算
	trans->identity_matrix(worldTransform_.matWorld_);

	

	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//worldTransform_.matWorld_ *= railCamera->GetWorldTransform().matWorld_;
	//worldTransform_.matWorld_.m[3][2] += 50.0f;
	//行列の転送
	worldTransform_.TransferMatrix();
}

void Player::Debug_Text() 
{
	//デバッグテキスト表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}
