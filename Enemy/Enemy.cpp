#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model ,Vector3& position) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("red.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�������W�̐ݒ�
	worldTransform_.translation_ = {position.x, position.y, position.z};

}

void Enemy::Update() 
{
	if (isDead_==false) {
		//�G�ړ��x�N�g��
		Vector3 move = {0, 0, 0};

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

		trans->identity_matrix(worldTransform_.matWorld_);
		trans->Affine_Trans(
		  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
		  worldTransform_.translation_);

		worldTransform_.TransferMatrix();
	}
	


}

void Enemy::Draw(const ViewProjection& viewProjection) 
{
	if(isDead_==false) {
		model_->Draw(worldTransform_, viewProjection, textureHandle_);
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
	//Vector3 enemy_pos = worldTransform_.translation_;
	Vector3 player_pos = player_->GetWorldPosition();
	Vector3 velocity = player_pos - enemy_pos;
	velocity.normalize();
	velocity *= kBulletSpeed;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = velocity * worldTransform_.matWorld_;

	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_, velocity);

	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
}

Vector3 Enemy::GetWorldPosition() 
{ 
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



void Enemy::OnCollision() 
{ isDead_ = true; }

