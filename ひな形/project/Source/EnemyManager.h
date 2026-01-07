#pragma once
#include "../Library/GameObject.h"
#include "zako1.h"
#include <vector>

class Enemy;
class BackGround;

enum class GamePhase {
    PHASE_1,    // パターン1の敵
    PHASE_2,    // パターン2の敵
    PHASE_3,    // パターン3の敵
    PHASE_BOSS, // ボス戦
    PHASE_CLEAR // 全クリア
};

struct EnemySpawnData {
    int enemyType;      // 0:雑魚, 1:ボス
    float spawnX;
    float spawnY;
    Zako1Pattern pattern;
};

class EnemyManager : public GameObject {
private:
    std::vector<Enemy*> enemies;
    std::vector<std::vector<EnemySpawnData>> phaseSpawnData; // フェーズごとのスポーンデータ
    BackGround* pBackground;
    GamePhase currentPhase;
    bool phaseSpawned; // 現在のフェーズの敵を出現させたか
    float phaseTransitionTimer; // フェーズ移行待機時間
    bool secondWaveSpawned; // 2番目の波を出現させたか
    float secondWaveTimer; // 2番目の波の出現タイマー

    void InitializeSpawnData();
    void SpawnEnemy(int type, float x, float y, Zako1Pattern pattern);
    void RemoveDeadEnemies();
    void SpawnPhaseEnemies(); // 現在のフェーズの敵を全て出現させる
    void SpawnSecondWave(); // 2番目の波を出現させる
    bool IsPhaseCleared(); // 現在のフェーズがクリアされたか
    void AdvancePhase(); // 次のフェーズに進む

public:
    EnemyManager(BackGround* bg);
    ~EnemyManager();
    void Update() override;
    void Draw() override;
    void Reset();

    GamePhase GetCurrentPhase() const { return currentPhase; }
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
};