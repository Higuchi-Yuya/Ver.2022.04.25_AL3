#include "Skydome.h"
#include "Affine_trans.h"

void Skydome::Initialize(Model* model) 
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("skydome/Fine_Basin.jpg");
	//���[���h�ϊ���������
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 250.0f;
	worldTransform_.scale_.y = 250.0f;
	worldTransform_.scale_.z = 250.0f;
}

void Skydome::Update() 
{ 
	Affine_trans* trans = nullptr;
	//�x�N�g���̉��Z
	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}
