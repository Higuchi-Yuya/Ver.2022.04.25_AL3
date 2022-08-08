#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class PlayerBullet 
{
  public:
	void Initialize(Model* model, const WorldTransform worldTransform, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// ���[���h�e���W���擾
	Vector3 GetBulletWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
