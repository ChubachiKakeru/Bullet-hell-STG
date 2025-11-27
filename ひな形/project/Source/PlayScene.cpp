#include "PlayScene.h"
#include"Player.h"
#include"Field.h"
#include"Boss1.h"
#include"BackGround.h"
//#include"CollisionManager.h"
#include"Bullet.h"
#include"zako1.h"
#include"Collision.h"
#include<vector>


PlayScene::PlayScene()
{
    new BackGround();
    new Field();

    //new Boss1();
    new zako1();
}

PlayScene::~PlayScene()
{
    // プレイヤーの弾を全て削除
    for (auto* bullet : playerBullets) {
        delete bullet;
    }
    playerBullets.clear();

    // ボスの弾を全て削除
    for (auto* bullet : bossBullets) {
        delete bullet;
    }
    bossBullets.clear();
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
    //// プレイヤーの弾を更新
    //for (Bullet* bullet : playerBullets) {
    //    bullet->Update();
    //}
      // 各オブジェクトの更新
    player.Update();
    boss.Update();

    // プレイヤーの弾を更新
    for (Bullet* bullet : playerBullets) {
        bullet->Update();
    }

    // ボスの弾を更新
    for (Bullet* bullet : bossBullets) {
        bullet->Update();
    }

    //// 当たり判定
    //CheckCollisions();

    //// 死んだ弾を削除
    //RemoveDeadBullets(playerBullets);
    //RemoveDeadBullets(bossBullets);
}

void PlayScene::CheckCollisions() {
    // ボスの弾 vs プレイヤー
    for (Bullet* bullet : bossBullets) {
        if (bullet->IsAlive() && player.IsAlive()) {
            if (Collision::MyCircleCheck(
                bullet->GetCenterX(),
                bullet->GetCenterY(),
                bullet->GetSize(),
                player.GetCenterX(),
                player.GetCenterY(),
                player.GetSize()
            )) {
                bullet->Hit();
                player.TakeDamage(10);
            }
        }
    }

    // プレイヤーの弾 vs ボス
    for (Bullet* bullet : playerBullets) {
        if (bullet->IsAlive() && boss.IsAlive()) {
            if (Collision::MyCircleCheck(
                bullet->GetCenterX(),
                bullet->GetCenterY(),
                bullet->GetSize(),
                boss.GetCenterX(),
                boss.GetCenterY(),
                boss.GetSize()
            )) {
                bullet->Hit();
                boss.TakeDamage(20);
            }
        }
    }
}

void PlayScene::RemoveDeadBullets(std::vector<Bullet*>& bullets)
{
    auto it = bullets.begin();
    while (it != bullets.end()) {
        if (!(*it)->IsAlive()) {
            delete* it;  // メモリ解放
            it = bullets.erase(it);  // vectorから削除
        }
        else {
            ++it;
        }
    }
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
    // プレイヤーとボスを描画
    player.Draw();
    boss.Draw();

    // プレイヤーの弾を描画
    for (Bullet* bullet : playerBullets) {
        bullet->Draw();
    }

    // ボスの弾を描画
    for (Bullet* bullet : bossBullets) {
        bullet->Draw();
    }
}

void PlayScene::FirePlayerBullet()
{
    // プレイヤーの弾を生成
    Bullet* newBullet = new Bullet(
        player.GetCenterX(),  // 開始X座標
        player.GetCenterY(),  // 開始Y座標
        0.0f,                 // X方向の速度
        -10.0f,               // Y方向の速度（上向き）
        6.0f                  // 弾のサイズ（半径）
    );

    playerBullets.push_back(newBullet);
}

void PlayScene::FireBoss1Bullet()
{
    // ボスの弾を生成
    Bullet* newBullet = new Bullet(
        boss.GetCenterX(),    // 開始X座標
        boss.GetCenterY(),    // 開始Y座標
        0.0f,                 // X方向の速度
        5.0f,                 // Y方向の速度（下向き）
        5.0f                  // 弾のサイズ（半径）
    );
}

