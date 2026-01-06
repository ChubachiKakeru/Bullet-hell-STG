#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss1.h"
#include "zako1.h"
#include "BackGround.h"
#include "Field.h"

namespace {
    constexpr float PATTERN1_SCROLL = 500.0f;
    constexpr float PATTERN2_SCROLL = 1500.0f;
    constexpr float PATTERN3_SCROLL = 2500.0f;
    constexpr float BOSS_SCROLL = 3500.0f;
    constexpr float ENEMY_SPACING = 150.0f;
}

EnemyManager::EnemyManager(BackGround* bg) : GameObject(),
pBackground(bg), currentSpawnIndex(0) {
    InitializeSpawnData();
}

EnemyManager::~EnemyManager() {
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
}

void EnemyManager::InitializeSpawnData() {
    spawnDataList.clear();
    float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;

    // パターン1: 左から3体横並び（真下に弾）
    for (int i = 0; i < 3; i++) {
        spawnDataList.push_back({
            PATTERN1_SCROLL,
            0,  // type 0 = zako1
            Field::STAGE_LEFT + 150.0f + (i * ENEMY_SPACING),
            Field::STAGE_TOP - 50.0f,
            false,
            Zako1Pattern::PATTERN_1
            });
    }

    // パターン2: 右から3体横並び（真下に弾）
    for (int i = 0; i < 3; i++) {
        spawnDataList.push_back({
            PATTERN2_SCROLL,
            0,
            Field::STAGE_RIGHT - 150.0f - (i * ENEMY_SPACING),
            Field::STAGE_TOP - 50.0f,
            false,
            Zako1Pattern::PATTERN_2
            });
    }

    // パターン3: 左から3体横並び（左右移動、自機狙い5way）
    for (int i = 0; i < 3; i++) {
        spawnDataList.push_back({
            PATTERN3_SCROLL,
            0,
            Field::STAGE_LEFT + 150.0f + (i * ENEMY_SPACING),
            Field::STAGE_TOP - 50.0f,
            false,
            Zako1Pattern::PATTERN_3
            });
    }

    // ボス（別タイミングで登場、独自の動きとフェーズ変化を持つ）
    spawnDataList.push_back({
        BOSS_SCROLL,
        1,  // type 1 = Boss1
        centerX - 60.0f,
        Field::STAGE_TOP + 20.0f,
        false,
        Zako1Pattern::PATTERN_1  // ボスは使用しない
        });

    currentSpawnIndex = 0;
}

void EnemyManager::Update() {
    if (!pBackground) return;
    float currentScroll = pBackground->GetscrollY();

    // デバッグ表示
    DrawFormatString(10, 60, GetColor(255, 255, 0),
        "Scroll: %.1f / Spawned: %d / Total: %d",
        currentScroll, currentSpawnIndex, spawnDataList.size());

    // 出現判定（スクロール値ベース）
    for (size_t i = currentSpawnIndex; i < spawnDataList.size(); i++) {
        EnemySpawnData& data = spawnDataList[i];

        // デバッグ表示：次の出現予定
        if (i == currentSpawnIndex) {
            DrawFormatString(10, 80, GetColor(255, 255, 0),
                "Next Spawn at: %.1f (Type: %d)",
                data.spawnScrollY, data.enemyType);
        }

        if (!data.hasSpawned && currentScroll >= data.spawnScrollY) {
            SpawnEnemy(data.enemyType, data.spawnX, data.spawnY, data.pattern);
            data.hasSpawned = true;
            currentSpawnIndex = i + 1;

            // スポーン成功のログ
            printfDx("敵出現: Type=%d, X=%.1f, Y=%.1f, Pattern=%d\n",
                data.enemyType, data.spawnX, data.spawnY, (int)data.pattern);
        }
        else {
            break;
        }
    }

    // 敵の数を表示
    DrawFormatString(10, 100, GetColor(255, 255, 0),
        "Active Enemies: %d", enemies.size());

    // 全敵の更新
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]) {
            enemies[i]->Update();

            // 各敵の座標を表示
            DrawFormatString(10, 120 + i * 20, GetColor(0, 255, 255),
                "Enemy[%d]: X=%.1f, Y=%.1f, Active=%d",
                i, enemies[i]->GetX(), enemies[i]->GetY(),
                !enemies[i]->IsDead());
        }
    }

    RemoveDeadEnemies();
}
void EnemyManager::Draw() {
    for (auto* enemy : enemies) {
        if (enemy) enemy->Draw();
    }
}

void EnemyManager::SpawnEnemy(int type, float x, float y, Zako1Pattern pattern) {
    Enemy* newEnemy = nullptr;
    switch (type) {
    case 0:  // 雑魚敵（パターン1,2,3の動きをする）
        newEnemy = new zako1(x, y, pattern);
        break;
    case 1:  // ボス（独自の動き：フェーズ1は固定、フェーズ2,3は左右移動）
        newEnemy = new Boss1(x, y);
        break;
    }
    if (newEnemy) enemies.push_back(newEnemy);
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
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    for (auto& data : spawnDataList) data.hasSpawned = false;
    currentSpawnIndex = 0;
}