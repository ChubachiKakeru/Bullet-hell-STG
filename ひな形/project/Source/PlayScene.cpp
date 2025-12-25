#include "PlayScene.h"
#include "Player.h"
#include "Field.h"
#include "Boss1.h"
#include "BackGround.h"
#include "Bullet.h"
#include "zako1.h"
#include "Boss2.h"

// ========================================
// コンストラクタ / デストラクタ
// ========================================
PlayScene::PlayScene()
{
    new BackGround();
    new Field();
    new zako1();
    new Boss2();
}

PlayScene::~PlayScene()
{
    // 弾の削除は各Bulletクラスのデストラクタで処理される想定
}

// ========================================
// 更新処理
// ========================================
void PlayScene::Update()
{
    // タイトルへの遷移
    if (CheckHitKey(KEY_INPUT_T)) {
        SceneManager::ChangeScene("TITLE");
    }

    // 注意: GameObjectシステムで各オブジェクトのUpdate()が自動的に呼ばれる想定
    // Player, Boss1, Boss2, zako1, playerBullet, enemyBullet等は
    // GameObjectManagerによって更新される
}

// ========================================
// 描画処理
// ========================================
void PlayScene::Draw()
{
    DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
    DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));

    // 注意: GameObjectシステムで各オブジェクトのDraw()が自動的に呼ばれる想定
}