#include "Enemy.h"
#include "Player.h"

void Enemy::Initialize(Model* model) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�������W�̐ݒ�
	worldTransform_.translation_ = {20, 3, 30};

}

void Enemy::Update() 
{
	//�G�ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });

	//�t�F�[�Y����
	switch (phase_) { 
	case Phase::Approach:
	default:
		Approach_Update();
		break;

	case Phase::Laeve:
		Laeve_Update();
		break;
	}

	//�e�X�V
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

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach_Initialize() 
{

}


void Enemy::Approach_Update() 
{
	//�G�ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�ړ�(�x�N�g�������Z)
	move = {0, 0, -enemy_speed_z};
	worldTransform_.translation_.z += move.z;

	//�K��̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < -5.0f) {
		phase_ = Phase::Laeve;
	}

	//���˃^�C�}�[���f�N�������g
	fireTimer_--;

	//�K�莞�Ԃɓ��B������
	if (fireTimer_ <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Laeve_Update() 
{
	//�G�ړ��x�N�g��
	Vector3 move = {0, 0, 0};
	//�ړ�(�x�N�g�������Z)
	move = {-enemy_speed_x, +enemy_speed_y, 0};
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
}

void Enemy::Fire() 
{
	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 0.1f;
	
	Vector3 enemy_pos = GetWorldPosition();
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity = player_pos - enemy_pos;
	velocity.normalize();
	velocity *= kBulletSpeed;

	////���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = velocity * worldTransform_.matWorld_;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

Vector3 Enemy::GetWorldPosition() 
{ 
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}



void Enemy::OnCollision() {}

