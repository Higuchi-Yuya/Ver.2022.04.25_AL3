﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <math.h>
#include <random>

#include "Enemy.h"
#include "Player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 Get_Reference_point(Vector3 &vertex);

	double Rad(double degree);

	void scale(Vector3& vertex, Vector3& reference_point, float expansion_rate_x, float expansion_rate_y,float expansion_rate_z);

	void translation(Vector3& vertex, float Tx, float Ty, float Tz);

	void rotate(Vector3& vertex, Vector3& reference_point, float frequency_x, float frequency_y, float frequency_z);

	void CheckAllCollisions();
  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	float grit_x = -20;
	float grit_z = -20;
	float Sx, Sy, Sz;
	float Rx[100], Ry[100], Rz[100];
	float Tx[100], Ty[100], Tz[100];

	float Eye_x, Eye_y, Eye_z = -50;
	float Target_x = 0, Target_y, Target_z;
	float viewAngle = 0.0f;

	float character_speed_x = 0.2f;

	float rotate_speed = 0.5f;

	//Vector3* Line = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	Player* player_ = nullptr;

	Enemy* enemy_;

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	Vector3 rotate_ = {0, 0, 0};
	Vector3 scale_ = {1, 1, 1};

	Vector3 grit_line_x_s[9];
	Vector3 grit_line_x_e[9];
	Vector3 grit_line_z_s[9];
	Vector3 grit_line_z_e[9];

	Vector4 grit_color_x = {0, 0, 1, 1};
	Vector4 grit_color_z = {1, 0, 0, 1};


	//ワールドトランスフォーム
	//WorldTransform worldTransform_[100];

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
