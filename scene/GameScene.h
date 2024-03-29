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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	float grit_x = -20;
	float grit_z = -20;
	//Vector3* Line = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	Vector3 box_vecter[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f}
    };

	Vector3 box_vecter2[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f}
    };

	Vector3 box_vecter3[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f}
    };

	Vector3 box_vecter4[8] = {
	  {0.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 0.0f},
      {5.0f, 0.0f, 5.0f},
      {0.0f, 0.0f, 5.0f},
	  {0.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 0.0f},
      {5.0f, 5.0f, 5.0f},
      {0.0f, 5.0f, 5.0f}
    };


	Vector3 grit_line_x_s[9];
	Vector3 grit_line_x_e[9];
	Vector3 grit_line_z_s[9];
	Vector3 grit_line_z_e[9];

	Vector4 grit_color_x = {0, 0, 1, 1};
	Vector4 grit_color_z = {1, 0, 0, 1};


	int vertex[12][2] = {{0,1}, {1,2}, {2,3}, {3,0}, {0,4}, {1,5}, {4,5}, {5,6}, {4,7}, {7,6}, {3,7}, {2,6}};
	Vector4 box_vecter_ = {1, 1, 1, 1};
	Vector4 box_vecter2_ = {1, 0, 0, 1};
	Vector4 box_vecter3_ = {1, 1, 0, 1};
	Vector4 box_vecter4_ = {1, 0, 1, 1};

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
