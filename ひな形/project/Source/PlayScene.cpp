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
    new Field();
    new zako1();
    new BackGround();

    boss1 = new Boss1();
    boss2 = nullptr;

    boss1Dead = false;
    boss2Spawned = false;
    boss2SpawnTimer = 0;
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

    // Boss1 がまだ生きているか監視
    if (!boss1Dead && boss1 && !boss1->IsAlive())
    {
        boss1Dead = true;
        boss2SpawnTimer = 0;
        printfDx("Boss1 撃破！Boss2出現準備開始\n");
    }

    // Boss1 撃破後、5秒待つ
    if (boss1Dead && !boss2Spawned)
    {
        boss2SpawnTimer++;

        // 5秒（60fps × 5）
        if (boss2SpawnTimer >= 60 * 5)
        {
            boss2 = new Boss2(300, 100);
            boss2Spawned = true;
            printfDx("Boss2 出現！\n");
        }
    }
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