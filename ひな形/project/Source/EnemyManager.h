#pragma once
#include<vector>
#include"../Library/GameObject.h"

class Enemy;
class BackGround;

struct EnemySpawnData {
    float spawnScrollY;
    int enemyType;
    float spawnX;
    float spawnY;
    bool hasSpawned;
};

class EnemyManager : public GameObject {
private:
    std::vector<Enemy*> enemies;
    std::vector<EnemySpawnData> spawnDataList;
    BackGround* pBackground;
    int currentSpawnIndex;

public:
    EnemyManager(BackGround* bg);
    ~EnemyManager();

    void Update();
    void Draw();
    void InitializeSpawnData();
    void SpawnEnemy(int type, float x, float y);
    void RemoveDeadEnemies();
    void Reset();

    const std::vector<Enemy*>& GetEnemies() const { return enemies; }
};
