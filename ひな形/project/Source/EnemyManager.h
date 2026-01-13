#pragma once
#include "../Library/GameObject.h"
#include "StageData.h"
#include <vector>
#include <string>

class Enemy;
class BackGround;

enum class GamePhase {
    PHASE_1 = 0,
    PHASE_2,
    PHASE_3,
    PHASE_BOSS,
    PHASE_CLEAR
};

class EnemyManager : public GameObject {
private:
    std::vector<Enemy*> enemies;
    BackGround* pBackground;

    // ステージデータ
    StageData* currentStageData;
    int currentStageNumber;

    // フェーズ管理
    GamePhase currentPhase;
    int currentPhaseIndex;

    // ウェーブ管理
    int currentWaveIndex;
    float waveTimer;
    bool waveSpawned;

    // フェーズ遷移管理
    float phaseTransitionTimer;
    bool phaseCleared;

    // 個別敵の遅延スポーン管理
    struct DelayedSpawn {
        EnemySpawnData data;
        float timer;
        bool spawned;
    };
    std::vector<DelayedSpawn> delayedSpawns;

public:
    EnemyManager(BackGround* bg);
    ~EnemyManager();

    void Update() override;
    void Draw() override;
    void Reset();

    // ステージ切り替え（CSVファイルから読み込み）
    bool LoadStageFromCSV(int stageNumber);

    // 敵管理
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }
    int GetEnemyCount() const { return enemies.size(); }
    GamePhase GetCurrentPhase() const { return currentPhase; }

private:
    void UpdatePhaseLogic();
    void UpdateWaveSpawning();
    void UpdateDelayedSpawns();
    void SpawnEnemy(const EnemySpawnData& data);
    void RemoveDeadEnemies();

    bool IsCurrentWaveCleared() const;
    bool IsPhaseCleared() const;
    void AdvanceToNextWave();
    void AdvanceToNextPhase();

    void DrawDebugInfo();
};