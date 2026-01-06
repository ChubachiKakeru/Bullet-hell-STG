#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss1.h"
#include "BackGround.h"

EnemyManager::EnemyManager(BackGround* bg) : GameObject() {
    pBackground = bg;
    currentSpawnIndex = 0;
    InitializeSpawnData();
}

EnemyManager::~EnemyManager() {
    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void EnemyManager::InitializeSpawnData() {
    spawnDataList.clear();

    // スクロール500pxで雑魚敵3体
  /*  spawnDataList.push_back({ 500.0f, 0, 200.0f, -50.0f, false });
    spawnDataList.push_back({ 500.0f, 0, 400.0f, -50.0f, false });
    spawnDataList.push_back({ 500.0f, 0, 600.0f, -50.0f, false });*/

    // スクロール3000pxでボス
    spawnDataList.push_back({ 3000.0f, 1, 420.0f, -100.0f, false });

    currentSpawnIndex = 0;
}

void EnemyManager::Update() {
    if (pBackground == nullptr) return;

    float currentScroll = pBackground->GetscrollY();

    // 出現判定
    for (size_t i = currentSpawnIndex; i < spawnDataList.size(); i++) {
        EnemySpawnData& data = spawnDataList[i];

        if (!data.hasSpawned && currentScroll >= data.spawnScrollY) {
            SpawnEnemy(data.enemyType, data.spawnX, data.spawnY);
            data.hasSpawned = true;
            currentSpawnIndex = i + 1;
        }
        else {
            break;
        }
    }

    // 全敵の更新
    for (auto* enemy : enemies) {
        if (enemy != nullptr) {
            enemy->Update();
        }
    }

    // 死んだ敵の削除
    RemoveDeadEnemies();
}

void EnemyManager::Draw() {
    for (auto* enemy : enemies) {
        if (enemy != nullptr) {
            enemy->Draw();
        }
    }
}

void EnemyManager::SpawnEnemy(int type, float x, float y) {
    Enemy* newEnemy = nullptr;

    switch (type) {
    case 0:  // 雑魚A（追加予定）
        // newEnemy = new EnemyZakoA(x, y);
        break;
    case 1:  // 雑魚B（追加予定）
        // newEnemy = new EnemyZakoB(x, y);
        break;
    case 2:  // Boss1
        newEnemy = new Boss1((int)x, (int)y);
        break;
    }

    if (newEnemy != nullptr) {
        enemies.push_back(newEnemy);
    }
}

void EnemyManager::RemoveDeadEnemies() {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it)->IsDead() || (*it)->IsOutOfScreen()) {
            delete* it;
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void EnemyManager::Reset() {
    // 全敵削除
    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // 出現データリセット
    for (auto& data : spawnDataList) {
        data.hasSpawned = false;
    }
    currentSpawnIndex = 0;
}