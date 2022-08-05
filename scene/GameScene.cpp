#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <algorithm>
#include <iostream>

const float Pi = 3.141593f;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
	delete player_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

Vector3 GameScene::Get_Reference_point(Vector3& vertex) { return vertex; }

double GameScene::Rad(double degree) { return degree * Pi / 180; }

void GameScene::scale(Vector3& vertex, Vector3& reference_point, float expansion_rate_x, float expansion_rate_y,float expansion_rate_z) {
	float w = 0;
	//拡大縮小
	vertex -= reference_point;
	vertex.x = (vertex.x * expansion_rate_x) + (vertex.y * 0) + (0 * vertex.z) + w;
	vertex.y = (vertex.x * 0) + (vertex.y * expansion_rate_y) + (0 * vertex.z) + w;
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * expansion_rate_z) + w;
	vertex += reference_point;
}

void GameScene::translation(Vector3& vertex, float Tx, float Ty, float Tz) {
	float w = 1;
	//移動
	vertex.x = (vertex.x * 1) + (vertex.y * 0) + (vertex.z * 0) + (Tx * w);
	vertex.y = (vertex.x * 0) + (vertex.y * 1) + (vertex.z * 0) + (Ty * w);
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * 1) + (Tz * w);
}

void GameScene::rotate(Vector3& vertex, Vector3& reference_point, float frequency_x, float frequency_y,float frequency_z) {
	float w = 0;
	float h_x, h_y, h_z;

	// xの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * 1) + (h_y * 0) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * cos(Rad(frequency_x))) + (h_z * sin(Rad(frequency_x))) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * -sin(Rad(frequency_x))) + (h_z * cos(Rad(frequency_x))) + (0 * w);
	vertex += reference_point;

	// yの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_y))) + (h_y * 0) + (h_z * -sin(Rad(frequency_y))) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * 1) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * sin(Rad(frequency_y))) + (h_y * 0) + (h_z * cos(Rad(frequency_y))) + (0 * w);
	vertex += reference_point;

	// zの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_z))) + (h_y * sin(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * -sin(Rad(frequency_z))) + (h_y * cos(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * 0) + (h_z * 1) + (0 * w);
	vertex += reference_point;
}

void GameScene::CheckAllCollisions() 
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	float radiusA = 1.0f, radiusB = 1.0f;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	#pragma region 自キャラと敵弾の当たり判定
	// 自キャラのワールド座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) 
	{
		// 敵弾のワールド座標
		posB = bullet->GetBulletWorldPosition();

		// 座標Aと座標Bの距離を求める
		float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
		float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
		float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
		float result_pos = result_pos_x + result_pos_y + result_pos_z;
		// 半径
		float result_radius = (radiusA + radiusB) * (radiusA + radiusB);

		if (result_pos <= result_radius) 
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラのワールド座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラ全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		// 敵弾のワールド座標
		posB = bullet->GetBulletWorldPosition();

		// 座標Aと座標Bの距離を求める
		float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
		float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
		float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
		float result_pos = result_pos_x + result_pos_y + result_pos_z;
		// 半径
		float result_radius = (radiusA + radiusB) * (radiusA + radiusB);

		if (result_pos <= result_radius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerbullet : playerBullets) {
		// 自弾のワールド座標
		posA = playerbullet->GetBulletWorldPosition();
		for (const std::unique_ptr<EnemyBullet>& enemybullet : enemyBullets) {
			// 敵弾のワールド座標
			posB = enemybullet->GetBulletWorldPosition();

			// 座標Aと座標Bの距離を求める
			float result_pos_x = (posB.x - posA.x) * (posB.x - posA.x);
			float result_pos_y = (posB.y - posA.y) * (posB.y - posA.y);
			float result_pos_z = (posB.z - posA.z) * (posB.z - posA.z);
			float result_pos = result_pos_x + result_pos_y + result_pos_z;
			// 半径
			float result_radius = (radiusA + radiusB) * (radiusA + radiusB);

			if (result_pos <= result_radius) {
				// 自弾の衝突時コールバックを呼び出す
				playerbullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemybullet->OnCollision();
			}
		}
	}
	#pragma endregion

}

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.jpg");

	debugCamera_ = new DebugCamera(1200, 720);

	//３Dモデルの生成
	model_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自キャラの生成
	player_ = new Player();

	//敵の生成
	enemy_ = new Enemy();

	skydome_ = new Skydome();

	// 天球の初期化
	skydome_->Initialize(modelSkydome_);

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の初期化
	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);



	////カメラ視点座標を設定
	// viewProjection_.eye = {Eye_x, Eye_y, Eye_z};

	////カメラ注視点座標を設定
	// viewProjection_.target = {Target_x, Target_y, Target_z};
	//
	////カメラ上方向ベクトルを設定 (右上45度指定)
	// viewProjection_.up = {cosf(Pi / 4.0f), sinf(Pi / 4.0f), 0.0f};

	////カメラ垂直方向視野角を設定
	// viewProjection_.fovAngleY = 50.0f * (Pi / 180);

	////アスペクト比を設定
	// viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定
	// viewProjection_.nearZ = 52.0f;

	////ファークリップ距離を設定
	// viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//乱数範囲の指定
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	

#pragma region グリット線の描画の設定
	//グリット線の描画位置設定

	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_s[i], grit_x, 0, -20);
		grit_x += 5;
	}
	grit_x = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_e[i], grit_x, 0, 20);
		grit_x += 5;
	}

	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_s[i], -20, 0, grit_z);
		grit_z += 5;
	}
	grit_z = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_e[i], 20, 0, grit_z);
		grit_z += 5;
	}
#pragma endregion

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

#pragma region キャラクター移動処理
	player_->Update();
	enemy_->Update();
	skydome_->Update();
#pragma endregion

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 天球の描画
	skydome_->Draw(viewProjection_);
	
	//ライン描画が参照するビュープロジェクションを指定する

	//グリット線の描画
	/*for (int i = 0; i < 9; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  grit_line_x_s[i], grit_line_x_e[i], grit_color_x);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  grit_line_z_s[i], grit_line_z_e[i], grit_color_z);
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
