#pragma once
#include "../Library/SceneBase.h"
#include "StageData.h"

// ゲームの状態を表す列挙型
enum class GameState
{
    COUNTDOWN,      // カウントダウン中
    PLAYING,        // ゲームプレイ中
    GAME_OVER,      // ゲームオーバー
    STAGE_CLEAR     // ステージクリア
};

class PlayScene : public SceneBase
{
public:
    PlayScene();
    virtual ~PlayScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    // ウェーブ生成処理
    void SpawnWave(StageData* stageData);

    // 各状態の更新処理
    void UpdateCountdown();     // カウントダウン更新
    void UpdatePlaying();       // ゲームプレイ更新
    void UpdateStageClear();    // ステージクリア更新

    // 各状態の描画処理
    void DrawCountdown();       // カウントダウン描画
    void DrawStageClear();      // ステージクリア描画

    // ゲーム状態
    GameState m_gameState = GameState::COUNTDOWN;

    // カウントダウン関連
    int m_countdownTimer;       // カウントダウンタイマー（フレーム数）
    int m_countdownNumber;      // 表示する数字（5, 4, 3, 2, 1）

    // ステージ遷移関連
    int m_nextStageTimer;       // 次ステージへの遷移タイマー

    // ステージデータ
    StageData* m_stageData = nullptr;
    int m_currentPhase;         // 現在のフェーズ番号
    int m_currentWave;          // 現在のウェーブ番号
    float m_phaseTimer;         // フェーズタイマー

    // サウンドパス
    const char* GAME_STAGE1_SOUND_PATH = "data/Sound/stage1.mp3";      //1面ジングル
    //  サウンドハンドル
    int stage1HitSoundHandle;		//	1面サウンドハンドル
};