#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = {
	  worldTransform.matWorld_.m[3][0], 
	  worldTransform.matWorld_.m[3][1],
	  worldTransform.matWorld_.m[3][2]};
}

void EnemyBullet::Update() 
{
	//���W���ړ�������(�P�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetBulletWorldPosition() 
{ 
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision() { isDead_ = true; }
