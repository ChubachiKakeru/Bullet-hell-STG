#pragma once
#include "../Library/GameObject.h"
#include "zako1.h"
#include <vector>

class Enemy;
class BackGround;

struct EnemySpawnData {
    float spawnScrollY;      // 出現するスクロール位置
    int enemyType;           // 敵の種類 (0:雑魚, 1:ボス)
    float spawnX;            // 出現X座標
    float spawnY;            // 出現Y座標
    bool hasSpawned;         // 出現済みフラグ
    Zako1Pattern pattern;    // 雑魚敵のパターン
};

class EnemyManager : public GameObject {
public:
    EnemyManager(BackGround* bg);
    ~EnemyManager();

    void Update() override;
    void Draw() override;
    void Reset();

    int GetEnemyCount() const { return enemies.size(); }
    bool IsBossActive() const;

private:
    BackGround* pBackground;
    std::vector<Enemy*> enemies;
    std::vector<EnemySpawnData> spawnDataList;
    size_t currentSpawnIndex;

    void InitializeSpawnData();
    void SpawnEnemy(int type, float x, float y, Zako1Pattern pattern);
    void RemoveDeadEnemies();
};