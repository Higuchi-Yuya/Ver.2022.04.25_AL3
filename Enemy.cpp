#include "Enemy.h"

void Enemy::Initialize(Model* model) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�������W�̐ݒ�
	worldTransform_.translation_ = {0, 3, 30};
}
void Enemy::Update() 
{
	//�G�ړ��x�N�g��
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
