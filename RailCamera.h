#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <cassert>

class RailCamera 
{
  public:
	void Initialize(WorldTransform worldTransform);

	void Update();

	ViewProjection GetViewProjection();

	WorldTransform* GetWorldTransform();


  private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// �r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	// �I�u�W�F�N�g�̓����X�s�[�h
	Vector3 moveSpeed_ = {0.01f, 0.01f, 0.04f};
	Vector3 rotateSpeed_ = {0.05f, 0.001f, 0.05f};
	DebugText* debugText_ = nullptr;
};
