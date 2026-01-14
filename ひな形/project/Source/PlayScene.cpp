#include "PlayScene.h"
#include "Player.h"
#include "Field.h"
#include "Boss1.h"
#include "BackGround.h"
#include "Bullet.h"
#include "zako1.h"
#include "Boss2.h"
#include "EnemyManager.h"
#include "Common.h"
#include "StageSelectScene.h"

// ========================================
// コンストラクタ / デストラクタ
// ========================================
PlayScene::PlayScene()
    : m_stageData(nullptr)
    , m_currentPhase(0)
    , m_currentWave(0)
    , m_phaseTimer(0.0f)
    , m_gameState(GameState::COUNTDOWN)
    , m_countdownTimer(180)  // 60fps × 3秒 = 180フレーム
    , m_countdownNumber(3)
{
    // ★背景と地形を先に生成（カウントダウン中も表示）★
    BackGround* bg = new BackGround();
    new Field();

    // ★自機を生成（カウントダウン中も表示・操作可能）★
    new Player();

    // ★ステージ選択画面で選んだステージ番号を取得★
    int selectedStage = StageSelectScene::GetSelectedStage();

    // ★EnemyManagerを生成するが、カウントダウン中は敵を出現させない★
    // EnemyManagerにカウントダウン状態を通知する方法が必要
    // 一旦、nullptrで生成を遅延させるか、EnemyManager側で制御
    new EnemyManager(bg, selectedStage);

    // ステージデータを取得（表示用）
    m_stageData = StageSelectScene::GetCurrentStageData();
    int stageNumber = StageSelectScene::GetSelectedStage();

    if (m_stageData != nullptr)
    {
        printfDx("ステージ%d 開始準備\n", stageNumber);
        printfDx("カウントダウン開始...\n");
        printfDx("フェーズ数: %d\n", m_stageData->phases.size());

        m_currentPhase = 0;
        m_currentWave = 0;
        m_phaseTimer = 0.0f;
    }
    else
    {
        printfDx("エラー: ステージデータが取得できませんでした\n");
    }
}

PlayScene::~PlayScene()
{
    // 弾の削除は各Bulletクラスのデストラクタで処理される想定
}

// ========================================
// ウェーブ生成処理
// ========================================
void PlayScene::SpawnWave(StageData* stageData)
{
    if (stageData == nullptr) return;
    if (m_currentPhase >= stageData->phases.size()) return;

    PhaseData& phase = stageData->phases[m_currentPhase];
    if (m_currentWave >= phase.waves.size()) return;

    WaveData& wave = phase.waves[m_currentWave];

    printfDx("フェーズ%d ウェーブ%d 開始 (敵数: %d)\n",
        m_currentPhase, m_currentWave, wave.enemies.size());

    // EnemyManagerを取得して敵を生成
    EnemyManager* em = FindGameObject<EnemyManager>();
    if (em)
    {
        for (const auto& enemyData : wave.enemies)
        {
            printfDx("敵生成: タイプ%d 座標(%.1f, %.1f) パターン%d 遅延%.1f秒\n",
                enemyData.enemyType,
                enemyData.spawnX,
                enemyData.spawnY,
                enemyData.pattern,
                enemyData.delayTime);
        }
    }
}

// ========================================
// 更新処理
// ========================================
void PlayScene::Update()
{
    // タイトルへの遷移
    if (CheckHitKey(KEY_INPUT_O)) {
        SceneManager::ChangeScene("TITLE");
        return;
    }

    // ゲーム状態に応じた更新
    switch (m_gameState)
    {
    case GameState::COUNTDOWN:
        UpdateCountdown();
        break;

    case GameState::PLAYING:
        UpdatePlaying();
        break;

    case GameState::GAME_OVER:
        // ゲームオーバー処理
        break;

    case GameState::STAGE_CLEAR:
        // ステージクリア処理
        break;
    }
}

// ========================================
// カウントダウン更新処理
// ========================================
void PlayScene::UpdateCountdown()
{
    // ★カウントダウン中もPlayer、Field、BackGroundは更新される★
    // （GameObjectシステムで自動的に更新される想定）

    // カウントダウンタイマーを減らす
    m_countdownTimer--;

    // カウントダウン数字の更新（60フレームごと）
    m_countdownNumber = (m_countdownTimer / 60) + 1;
    if (m_countdownNumber < 0) m_countdownNumber = 0;

    // カウントダウン終了
    if (m_countdownTimer <= 0)
    {
        m_gameState = GameState::PLAYING;
        printfDx("ステージ%d 本格開始！\n", StageSelectScene::GetSelectedStage());

        // ★敵の出現を開始する合図をEnemyManagerに送る★
        EnemyManager* em = FindGameObject<EnemyManager>();
        if (em)
        {
            em->StartGame();  // ★ゲーム開始を通知★
            printfDx("EnemyManager: ゲーム開始！\n");
        }

        //PlaySoundMem(/* スタート音 */, DX_PLAYTYPE_BACK);
    }
}

// ========================================
// ゲーム中の更新処理
// ========================================
void PlayScene::UpdatePlaying()
{
    // ★通常のゲームプレイ★
    // GameObjectシステムで各オブジェクトのUpdate()が自動的に呼ばれる想定
    // Player, EnemyManager, Boss, zako1, Bullet等

    // ステージデータがある場合の進行管理（オプション）
    // if (m_stageData != nullptr) {
    //     // フェーズ進行のロジックをここに追加可能
    // }
}

// ========================================
// 描画処理
// ========================================
void PlayScene::Draw()
{
    // ★ゲーム状態に応じた描画★
    if (m_gameState == GameState::COUNTDOWN)
    {
        // カウントダウン中の描画
        DrawCountdown();
    }
    else
    {
        // 通常のゲーム情報表示
        DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
        DrawString(100, 550, "Push [O]Key To Title", GetColor(255, 255, 255));

        // ステージ番号の表示
        if (m_stageData != nullptr)
        {
            int stageNumber = StageSelectScene::GetSelectedStage();
            DrawFormatString(10, 50, GetColor(255, 255, 255),
                "Stage: %d | Phase: %d/%d",
                stageNumber,
                m_currentPhase + 1,
                m_stageData->phases.size());
        }

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
                    "Press [O] to return to Title");
                break;
            }
        }
    }

    // 注意: GameObjectシステムで各オブジェクトのDraw()が自動的に呼ばれる想定
    // 背景、自機、敵、弾などは常に描画される
}

// ========================================
// カウントダウン描画処理
// ========================================
void PlayScene::DrawCountdown()
{
    // ステージ番号表示
    int stageNumber = StageSelectScene::GetSelectedStage();
    SetFontSize(48);
    DrawFormatString(400, 150, GetColor(255, 255, 255),
        "=== STAGE %d ===", stageNumber);
    SetFontSize(16);

    // カウントダウン数字を大きく表示
    if (m_countdownNumber > 0)
    {
        // タイマーから進行度を計算（0.0～1.0）
        float progress = (float)(m_countdownTimer % 60) / 60.0f;

        // 拡大縮小アニメーション（1.2 → 1.0）
        float scale = 1.0f + 0.2f * progress;

        // 透明度アニメーション（255 → 180）
        int alpha = (int)(180 + 75 * progress);

        SetFontSize((int)(150 * scale));

        unsigned int color = GetColor(255, 255, alpha);

        char numStr[8];
        sprintf_s(numStr, "%d", m_countdownNumber);
        int textWidth = GetDrawStringWidth(numStr, (int)strlen(numStr));

        int yOffset = (int)(20 * (scale - 1.0f));
        DrawString(640 - textWidth / 2, 280 - yOffset, numStr, color);

        SetFontSize(16);
    }
    else if (m_countdownTimer > -30) // スタート表示を30フレーム表示
    {
        // "START!" のアニメーション
        float progress = 1.0f + (float)m_countdownTimer / 30.0f;
        if (progress < 0) progress = 0;

        float scale = 1.0f + 0.3f * progress;
        int alpha = (int)(255 * progress);

        SetFontSize((int)(100 * scale));
        const char* startText = "START!";
        int textWidth = GetDrawStringWidth(startText, (int)strlen(startText));
        DrawString(640 - textWidth / 2, 300, startText,
            GetColor(255, 255 - (255 - alpha), alpha));
        SetFontSize(16);
    }

    // 操作説明
    SetFontSize(24);
    DrawString(380, 480, "WASDキー: 移動 / Hキー: ショット",
        GetColor(200, 200, 200));
    DrawString(450, 520, "カウント中も操作可能！",
        GetColor(255, 255, 100));
    SetFontSize(16);
}