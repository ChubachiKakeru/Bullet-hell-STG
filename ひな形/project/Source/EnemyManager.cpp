#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss1.h"
#include "zako1.h"
#include "BackGround.h"
#include "Field.h"

namespace {
    constexpr float ENEMY_SPACING = 150.0f;
    constexpr float PHASE_TRANSITION_WAIT = 2.0f; // フェーズ移行の待機時間（秒）
}

EnemyManager::EnemyManager(BackGround* bg) : GameObject(),
pBackground(bg),
currentPhase(GamePhase::PHASE_1),
phaseSpawned(false),
phaseTransitionTimer(0.0f),
secondWaveSpawned(false),
secondWaveTimer(0.0f) {
    InitializeSpawnData();
}

EnemyManager::~EnemyManager() {
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
}

void EnemyManager::InitializeSpawnData() {
    phaseSpawnData.clear();
    phaseSpawnData.resize(4); // 4つのフェーズ分

    float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;

    // フェーズ1: 左から3体横並び（真下に弾）
    //for (int i = 0; i < 3; i++) {
    //    phaseSpawnData[0].push_back({
    //        0,  // type 0 = zako1
    //        Field::STAGE_LEFT + 150.0f + (i * ENEMY_SPACING),
    //        Field::STAGE_TOP + 50.0f,
    //        Zako1Pattern::PATTERN_LEFT_TO_RIGHT
    //        });
    //}
    phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + -100.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 50.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 200.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 350.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 500.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });


    //// フェーズ2: 右から3体横並び（真下に弾）
    //for (int i = 0; i < 3; i++) {
    //    phaseSpawnData[1].push_back({
    //        0,
    //        Field::STAGE_RIGHT - /*150.0f -*/ (i * ENEMY_SPACING),
    //        Field::STAGE_TOP + 50.0f,
    //        Zako1Pattern::PATTERN_RIGHT_TO_LEFT
    //        });
    //}
    phaseSpawnData[1].push_back({ 0, Field::STAGE_LEFT + 190.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT });
    phaseSpawnData[1].push_back({ 0, Field::STAGE_LEFT + 340.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT });
    phaseSpawnData[1].push_back({ 0, Field::STAGE_LEFT + 490.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT });
    phaseSpawnData[1].push_back({ 0, Field::STAGE_LEFT + 640.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT });
    phaseSpawnData[1].push_back({ 0, Field::STAGE_LEFT + 790.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT });
    // フェーズ3: 左から3体横並び（真下に弾）
    // ※2秒後に右から3体追加される（SpawnSecondWaveで処理）
    /*for (int i = 0; i < 3; i++) {
        phaseSpawnData[2].push_back({
            0,
            Field::STAGE_LEFT - 150.0f + (i * ENEMY_SPACING),
            Field::STAGE_TOP + 70.0f,
            Zako1Pattern::PATTERN_LEFT_TO_RIGHT
            });
    }*/
    phaseSpawnData[2].push_back({ 0, Field::STAGE_LEFT + -100.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[2].push_back({ 0, Field::STAGE_LEFT + 50.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[2].push_back({ 0, Field::STAGE_LEFT + 200.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[2].push_back({ 0, Field::STAGE_LEFT + 350.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    phaseSpawnData[2].push_back({ 0, Field::STAGE_LEFT + 500.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
    // フェーズ4: ボス
    phaseSpawnData[3].push_back({1,  /* type 1 = Boss1*/centerX - 60.0f,Field::STAGE_TOP + 20.0f,Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
}

void EnemyManager::Update() {
    if (!pBackground) return;

    // フェーズ表示
    const char* phaseNames[] = { "PHASE 1", "PHASE 2", "PHASE 3", "BOSS PHASE", "CLEAR!" };
    int phaseIndex = static_cast<int>(currentPhase);
    DrawFormatString(10, 60, GetColor(255, 255, 0),
        "Current Phase: %s", phaseNames[phaseIndex]);

    // 現在のフェーズの敵をまだ出現させていなければ出現させる
    if (!phaseSpawned && currentPhase != GamePhase::PHASE_CLEAR) {
        SpawnPhaseEnemies();
        phaseSpawned = true;
        phaseTransitionTimer = 0.0f;
        secondWaveSpawned = false;
        secondWaveTimer = 0.0f;
    }

    // フェーズ3の場合のみ、2秒後に右から敵を出現させる
    if (phaseSpawned && !secondWaveSpawned && currentPhase == GamePhase::PHASE_3) {
        secondWaveTimer += 1.0f / 60.0f;

        if (secondWaveTimer >= 2.0f) {
            SpawnSecondWave();
            secondWaveSpawned = true;
        }
    }

    // 敵の数を表示
    DrawFormatString(10, 80, GetColor(255, 255, 0),
        "Active Enemies: %d", enemies.size());

    // 全敵の更新
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]) {
            enemies[i]->Update();

            // 各敵の座標を表示（最大5体まで）
            if (i < 5) {
                DrawFormatString(10, 100 + i * 20, GetColor(0, 255, 255),
                    "Enemy[%d]: X=%.1f, Y=%.1f, HP=%d",
                    i, enemies[i]->GetX(), enemies[i]->GetY(),
                    enemies[i]->GetHP());
            }
        }
    }

    RemoveDeadEnemies();

    // フェーズクリア判定
    if (phaseSpawned && IsPhaseCleared()) {
        phaseTransitionTimer += 1.0f / 60.0f; // 60FPS想定

        // 待機時間表示
        DrawFormatString(10, 200, GetColor(0, 255, 0),
            "Phase Cleared! Next phase in %.1f sec...",
            PHASE_TRANSITION_WAIT - phaseTransitionTimer);

        // 一定時間後に次のフェーズへ
        if (phaseTransitionTimer >= PHASE_TRANSITION_WAIT) {
            AdvancePhase();
        }
    }

    // ゲームクリア表示
    if (currentPhase == GamePhase::PHASE_CLEAR) {
        DrawFormatString(10, 220, GetColor(255, 255, 0),
            "=== GAME CLEAR ===");
    }
}

void EnemyManager::Draw() {
    for (auto* enemy : enemies) {
        if (enemy) enemy->Draw();
    }
}

void EnemyManager::SpawnPhaseEnemies() {
    int phaseIndex = static_cast<int>(currentPhase);

    if (phaseIndex < 0 || phaseIndex >= phaseSpawnData.size()) return;

    // フェーズ3は左から3体のみ（第1波）、その他は全て出現
    if (currentPhase == GamePhase::PHASE_3) {
        // 左から3体のみ出現（最初の3体）
        for (size_t i = 0; i < 3 && i < phaseSpawnData[phaseIndex].size(); i++) {
            const auto& data = phaseSpawnData[phaseIndex][i];
            SpawnEnemy(data.enemyType, data.spawnX, data.spawnY, data.pattern);

            printfDx("フェーズ3（第1波）: 敵出現 Type=%d, X=%.1f, Y=%.1f\n",
                data.enemyType, data.spawnX, data.spawnY);
        }
    }
    else {
        // その他のフェーズは通常通り全て出現
        for (const auto& data : phaseSpawnData[phaseIndex]) {
            SpawnEnemy(data.enemyType, data.spawnX, data.spawnY, data.pattern);

            printfDx("フェーズ%d: 敵出現 Type=%d, X=%.1f, Y=%.1f\n",
                phaseIndex + 1, data.enemyType, data.spawnX, data.spawnY);
        }
    }
}

void EnemyManager::SpawnSecondWave() {
    // 右から5体を出現させる
    float rightStart = Field::STAGE_RIGHT + 100.0f;

    for (int i = 0; i < 5; i++) {
        SpawnEnemy(0, rightStart - (i * ENEMY_SPACING), Field::STAGE_TOP + 150.0f, Zako1Pattern::PATTERN_RIGHT_TO_LEFT);

        printfDx("第2波: 敵出現 X=%.1f, Y=%.1f\n",
            rightStart - (i * ENEMY_SPACING), Field::STAGE_TOP + 50.0f);
    }
}

bool EnemyManager::IsPhaseCleared() {
    // 敵が全滅したらフェーズクリア
    return enemies.empty();
}

void EnemyManager::AdvancePhase() {
    switch (currentPhase) {
    case GamePhase::PHASE_1:
        currentPhase = GamePhase::PHASE_2;
        break;
    case GamePhase::PHASE_2:
        currentPhase = GamePhase::PHASE_3;
        break;
    case GamePhase::PHASE_3:
        currentPhase = GamePhase::PHASE_BOSS;
        break;
    case GamePhase::PHASE_BOSS:
        currentPhase = GamePhase::PHASE_CLEAR;
        break;
    case GamePhase::PHASE_CLEAR:
        // すでにクリア済み
        break;
    }

    phaseSpawned = false;
    phaseTransitionTimer = 0.0f;
    secondWaveSpawned = false;
    secondWaveTimer = 0.0f;

    printfDx("フェーズ移行: Phase %d\n", static_cast<int>(currentPhase) + 1);
}

void EnemyManager::SpawnEnemy(int type, float x, float y, Zako1Pattern pattern) {
    Enemy* newEnemy = nullptr;
    switch (type) {
    case 0:  // 雑魚敵（float型に対応）
        newEnemy = new zako1(x, y, pattern);
        break;
    case 1:  // ボス（float型に対応）
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
    currentPhase = GamePhase::PHASE_1;
    phaseSpawned = false;
    phaseTransitionTimer = 0.0f;
    secondWaveSpawned = false;
    secondWaveTimer = 0.0f;
}