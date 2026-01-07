#include "PlayScene.h"
#include "Player.h"
#include "Field.h"
#include "Boss1.h"
#include "BackGround.h"
#include "Bullet.h"
#include "zako1.h"
#include "Boss2.h"
#include "EnemyManager.h"

// ========================================
// コンストラクタ / デストラクタ
// ========================================
PlayScene::PlayScene()
{
    BackGround* bg = new BackGround();
    new Field();
    new Player();
    new EnemyManager(bg);  // BackGroundを渡してEnemyManager生成
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
    // 基本情報
    DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
    DrawString(100, 550, "Push [T]Key To Title", GetColor(255, 255, 255));

    // フェーズ情報とゲーム状態の表示
    EnemyManager* em = FindGameObject<EnemyManager>();
    if (em) {
        GamePhase currentPhase = em->GetCurrentPhase();

        // フェーズ名を画面中央上部に大きく表示
        const char* phaseNames[] = {
            "=== PHASE 1 ===",
            "=== PHASE 2 ===",
            "=== PHASE 3 ===",
            "=== BOSS PHASE ===",
            "=== GAME CLEAR ==="
        };
        int phaseIndex = static_cast<int>(currentPhase);
        DrawFormatString(250, 10, GetColor(255, 255, 0),
            "%s", phaseNames[phaseIndex]);

        // 敵の数を表示
        DrawFormatString(10, 10, GetColor(255, 255, 255),
            "Enemies: %d", em->GetEnemyCount());

        // フェーズごとの説明を表示
        switch (currentPhase) {
        case GamePhase::PHASE_1:
            DrawFormatString(10, 30, GetColor(200, 200, 200),
                "3 enemies moving left to right");
            break;
        case GamePhase::PHASE_2:
            DrawFormatString(10, 30, GetColor(200, 200, 200),
                "3 enemies moving right to left");
            break;
        case GamePhase::PHASE_3:
            DrawFormatString(10, 30, GetColor(200, 200, 200),
                "6 enemies - Wave 1 then Wave 2");
            break;
        case GamePhase::PHASE_BOSS:
            DrawFormatString(10, 30, GetColor(255, 100, 100),
                "DEFEAT THE BOSS!");
            break;
        case GamePhase::PHASE_CLEAR:
            // ゲームクリア画面
            DrawFormatString(220, 250, GetColor(255, 255, 0),
                "*** CONGRATULATIONS ***");
            DrawFormatString(280, 280, GetColor(255, 255, 255),
                "YOU WIN!");
            DrawFormatString(240, 320, GetColor(200, 200, 200),
                "Press [T] to return to Title");
            break;
        }
    }

    // 注意: GameObjectシステムで各オブジェクトのDraw()が自動的に呼ばれる想定
}