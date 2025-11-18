#include "PlayScene.h"
#include"Player.h"
#include"Field.h"
#include"Boss1.h"
#include"BackGround.h"
#include"CollisionManager.h"
#include"Bullet.h"
#include"zako1.h"


PlayScene::PlayScene()
{
	new BackGround();
	new Field();
	new Boss1();
    new zako1();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}

	// プレイヤー更新
	Player* player = FindGameObject<Player>();
	if (player != nullptr) {
		player->Update();
	}

	// ボス更新
	Boss1* boss = FindGameObject<Boss1>();
	if (boss != nullptr) {
		boss->Update();
	}

	// 弾の判定処理
	CheckCollisions();
}

void PlayScene::CheckCollisions() {
    Player* player = FindGameObject<Player>();
    Boss1* boss = FindGameObject<Boss1>();

    if (player == nullptr || !player->IsActive()) return;

    // プレイヤーの弾 vs ボス
    if (boss != nullptr && boss->IsActive()) {
        // 全ての弾をチェック（仮想コード）
       /*  for (Bullet* bullet : playerBullets) {
             if (bullet->IsActive()) {
                 bool hit = CollisionManager::CheckCollision(
                     bullet->GetX(), bullet->GetY(), 
                     bullet->GetRectW(), bullet->GetRectH(), bullet->GetRadius(),
                     boss->GetX(), boss->GetY(), 
                     boss->GetRectW(), boss->GetRectH(), boss->GetRadius()
                 );
                 
                 if (hit) {
                     boss->TakeDamage(10);
                     bullet->Deactivate();
                 }
             }
         }*/
    }
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}

