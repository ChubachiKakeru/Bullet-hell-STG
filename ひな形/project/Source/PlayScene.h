#pragma once
#include "../Library/SceneBase.h"
#include "StageData.h"

class PlayScene : public SceneBase
{
public:
    PlayScene();
    virtual ~PlayScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    // ★ゲーム状態★
    enum class GameState
    {
        COUNTDOWN,      // カウントダウン中（自機操作可能、敵未出現）
        PLAYING,        // ゲーム中
        GAME_OVER,      // ゲームオーバー
        STAGE_CLEAR     // ステージクリア
    };

    // ★メンバ関数の宣言★
    void UpdateCountdown();     // カウントダウン更新
    void UpdatePlaying();       // ゲーム中の更新
    void DrawCountdown();       // カウントダウン描画
    void SpawnWave(StageData* stageData);

    // ★メンバ変数★
    GameState m_gameState;      // 現在のゲーム状態
    int m_countdownTimer;       // カウントダウンタイマー（フレーム数）
    int m_countdownNumber;      // 表示するカウントダウン数字（3, 2, 1）

    StageData* m_stageData;
    int m_currentPhase;
    int m_currentWave;
    float m_phaseTimer;
};