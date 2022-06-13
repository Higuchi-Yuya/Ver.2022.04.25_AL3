#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class EnemyBullet 
{
  public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 4;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};