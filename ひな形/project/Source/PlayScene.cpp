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
    //// プレイヤーの弾を更新
    //for (Bullet* bullet : playerBullets) {
    //    bullet->Update();
    //}
    
}

void PlayScene::CheckCollisions() {
    for (auto& bullet : playerBullets) {
        if (bullet.IsAlive() && boss.IsAlive()) {
            if (Collision::MyCircleCheck(
                bullet.GetCenterX(),
                bullet.GetCenterY(),
                bullet.GetSize(),
                boss.GetCenterX(),
                boss.GetCenterY(),
                boss.GetSize()
            )) {
                bullet.Hit();
                boss.TakeDamage(20);
            }
        }
    }
}

//void PlayScene::RemoveDeadBullets(std::vector<Bullet*>& bullets)
//{
//}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}

