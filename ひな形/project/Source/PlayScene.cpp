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
#include "DxLib.h"
#include <string.h>  // strlen用

// ========================================
// コンストラクタ / デストラクタ
// ========================================

PlayScene::PlayScene() {
    // ★背景と地形を先に生成（カウントダウン中も表示）★
    BackGround* bg = new BackGround();
    new Field();

    // ★自機を生成（カウントダウン中も表示・操作可能）★
    new Player();

    // ★ステージ選択画面で選んだステージ番号を取得★
    int selectedStage = StageSelectScene::GetSelectedStageNumber();

    // ★ステージごとにカウントダウン時間を設定★
    switch (selectedStage)
    {
    case 1:
        PlaySoundFile("data/Sound/stage1.mp3", DX_PLAYTYPE_LOOP);
        m_countdownTimer = 60 * 5;  // 5秒
        m_countdownNumber = 5;
        break;
    case 2:
        PlaySoundFile("data/Sound/stage2.mp3", DX_PLAYTYPE_LOOP);
        m_countdownTimer = 60 * 3;  // 3秒
        m_countdownNumber = 3;
        break;
    default:
        m_countdownTimer = 60 * 3;  // デフォルト3秒
        m_countdownNumber = 3;
        break;
    }

    // ★EnemyManagerを生成するが、カウントダウン中は敵を出現させない★
    new EnemyManager(bg, selectedStage);

    // ステージデータを取得（表示用）
    m_stageData = StageSelectScene::GetCurrentStageData();
    int stageNumber = StageSelectScene::GetSelectedStageNumber();

    if (m_stageData != nullptr)
    {
        m_currentPhase = 0;
        m_currentWave = 0;
        m_phaseTimer = 0.0f;
    }

    // ★次ステージ遷移タイマーの初期化★
    m_nextStageTimer = 0;
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

    // EnemyManagerを取得して敵を生成
    EnemyManager* em = FindGameObject<EnemyManager>();
    if (em)
    {
        for (const auto& enemyData : wave.enemies)
        {
            // 敵生成処理
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
        UpdateStageClear();
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

        // ★敵の出現を開始する合図をEnemyManagerに送る★
        EnemyManager* em = FindGameObject<EnemyManager>();
        if (em)
        {
            em->StartGame();  // ★ゲーム開始を通知★
        }

        //PlaySoundMem(/* スタート音 */, DX_PLAYTYPE_BACK);
    }
}

// ========================================
// ゲーム中の更新処理
// ========================================
void PlayScene::UpdatePlaying()
{
    // ★ゲーム全体のタイマーを増やす★
    StageSelectScene::s_totalGameTimer++;

    // 既存のコード...
    EnemyManager* em = FindGameObject<EnemyManager>();
    if (em && em->GetCurrentPhase() == GamePhase::PHASE_CLEAR)
    {
        m_gameState = GameState::STAGE_CLEAR;
        m_nextStageTimer = 60 * 3;
    }
}

// ========================================
// ステージクリア更新処理
// ========================================
void PlayScene::UpdateStageClear()
{
    m_nextStageTimer--;

    // タイマーが0になったら次のステージへ
    if (m_nextStageTimer <= 0)
    {
        int currentStage = StageSelectScene::GetSelectedStageNumber();

        if (currentStage == 1) // ステージ1クリア後はショップへ
        {
            // ★ステージ番号を2に更新してからショップへ★
            StageSelectScene::GoToNextStage();
        }
        else if (currentStage == 2) // ステージ2クリア後はクリア画面へ
        {
            SceneManager::ChangeScene("CLEAR");
        }
        else // それ以外（念のため）
        {
            SceneManager::ChangeScene("CLEAR");
        }
    }
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
    else if (m_gameState == GameState::STAGE_CLEAR)
    {
        // ステージクリア画面
        DrawStageClear();
    }
    else
    {
        // 通常のゲーム情報表示
        DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
        DrawString(100, 550, "Push [O]Key To Title", GetColor(255, 255, 255));

        // ステージ番号の表示
        if (m_stageData != nullptr)
        {
            int stageNumber = StageSelectScene::GetSelectedStageNumber();
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
                // ゲームクリア画面はDrawStageClear()で描画
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
    int stageNumber = StageSelectScene::GetSelectedStageNumber();
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

        int yOffset = (int)(20 * (scale - 1.0f));
        DrawFormatString(580, 280 - yOffset, color, "%d", m_countdownNumber);

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
        unsigned int color = GetColor(255, 255 - (255 - alpha), alpha);

        DrawString(520, 300, "START!", color);
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

// ========================================
// ステージクリア描画処理
// ========================================
void PlayScene::DrawStageClear()
{
    int currentStage = StageSelectScene::GetSelectedStageNumber();
    float timeLeft = m_nextStageTimer / 60.0f;

    // 背景を少し暗くする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(0, 0, 1280, 1280, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // クリアメッセージ
    SetFontSize(60);
    DrawFormatString(350, 200, GetColor(255, 255, 0),
        "STAGE %d CLEAR!", currentStage);
    SetFontSize(16);

    // 次のステージへの案内
    if (currentStage == 1)
    {
        // ★ステージ1クリア後はショップへ★
        SetFontSize(36);
        DrawString(480, 300, "Next: SHOP", GetColor(255, 255, 255));
        SetFontSize(16);

        // カウントダウン表示
        SetFontSize(48);
        DrawFormatString(550, 380, GetColor(200, 200, 200),
            "%.1f", timeLeft);
        SetFontSize(16);
    }
    else if (currentStage == 2)
    {
        // 全ステージクリア
        SetFontSize(48);
        DrawString(320, 300, "ALL STAGES CLEAR!", GetColor(255, 255, 0));
        SetFontSize(16);

        SetFontSize(24);
        DrawFormatString(420, 380, GetColor(200, 200, 200),
            "Moving to Clear Scene... %.1f", timeLeft);
        SetFontSize(16);
    }
}