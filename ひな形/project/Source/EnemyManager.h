#pragma once
#include "../Library/GameObject.h"
#include "StageData.h"
#include <vector>

class Enemy;
class BackGround;
class zako1;
class zako2;
class Boss1;
class Boss2;

// ゲーム全体のフェーズ（旧システムとの互換性用）
enum class GamePhase {
    PHASE_1,
    PHASE_2,
    PHASE_3,
    PHASE_BOSS,
    PHASE_CLEAR
};

// 遅延出現用の構造体
struct DelayedSpawn {
    EnemySpawnData data;
    float timer;
    bool spawned;
};

class EnemyManager : public GameObject
{
public:
    EnemyManager(BackGround* bg, int stageNumber = 1);
    virtual ~EnemyManager();

    void Update() override;
    void Draw() override;
    void Reset();

    // ★追加: ゲーム開始制御★
    void StartGame() { m_gameStarted = true; }
    bool IsGameStarted() const { return m_gameStarted; }

    // ステージ読み込み
    bool LoadStageFromCSV(int stageNumber);

    // 敵の情報取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
    GamePhase GetCurrentPhase() const { return currentPhase; }
    int GetCurrentStageNumber() const { return currentStageNumber; }

    // 敵リストの取得（当たり判定用）
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }

private:
    BackGround* pBackground;

    // ★追加: ゲーム開始フラグ★
    bool m_gameStarted;

    // ステージデータ
    StageData* currentStageData;
    int currentStageNumber;

    // 敵管理
    std::vector<Enemy*> enemies;
    std::vector<DelayedSpawn> delayedSpawns;

    // フェーズ管理
    GamePhase currentPhase;
    int currentPhaseIndex;
    int currentWaveIndex;
    float waveTimer;
    bool waveSpawned;
    float phaseTransitionTimer;
    bool phaseCleared;

    // 内部処理
    void UpdatePhaseLogic();
    void UpdateWaveSpawning();
    void UpdateDelayedSpawns();
    void SpawnEnemy(const EnemySpawnData& data);
    bool IsCurrentWaveCleared() const;
    bool IsPhaseCleared() const;
    void AdvanceToNextWave();
    void AdvanceToNextPhase();
    void RemoveDeadEnemies();
    void DrawDebugInfo();
};