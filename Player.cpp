#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle) { 
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換を初期化
	worldTransform_.Initialize();
}

void Player::Update() {

	Affine_trans* trans = nullptr;
	constexpr float MoveLimitX = 35;
	constexpr float MoveLimitY = 19;

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
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

	//ベクトルの加算
	trans->translation(worldTransform_.matWorld_, worldTransform_.translation_);

	//行列の転送
	worldTransform_.TransferMatrix();

	//デバッグテキスト表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Pos:(%f,%f,%f)", worldTransform_.translation_.x,
	  worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
