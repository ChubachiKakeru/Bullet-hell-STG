#include "PlayScene.h"
#include"Player.h"
#include"Field.h"
#include"Boss1.h"
#include"BackGround.h"
//#include"CollisionManager.h"
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

	
}

void PlayScene::CheckCollisions() {
   
}

//void PlayScene::RemoveDeadBullets(std::vector<Bullet*>& bullets)
//{
//}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}

