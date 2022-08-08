#include "Player.h"
#include "RailCamera.h"

//RailCamera* railCamera = nullptr;

Player::~Player() 
{
	
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ���������
	worldTransform_.translation_.z = 50.0f;
	worldTransform_.Initialize();
	



	
}

void Player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//��]����
	Rotation();

	//�ړ�����
	Move();

	//�L�����N�^�[�̍U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	

	//�s��̍X�V����ѓ]��
	Trans_Update();

	//�f�o�b�O�e�L�X�g
	Debug_Text();
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_Y)) {

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		Affine_trans trans;

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = velocity * worldTransform_.matWorld_;

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() 
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
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

	//�L�����N�^�[�̈ړ�����
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

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MoveLimitY);
}

void Player::Rotation() 
{ 
	Vector3 rotate = {0, 0, 0};

	//�L�����N�^�[�̐���ړ�
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
	
	//�x�N�g���̉��Z
	trans->identity_matrix(worldTransform_.matWorld_);

	

	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//worldTransform_.matWorld_ *= railCamera->GetWorldTransform().matWorld_;
	//worldTransform_.matWorld_.m[3][2] += 50.0f;
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Player::Debug_Text() 
{
	//�f�o�b�O�e�L�X�g�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}
