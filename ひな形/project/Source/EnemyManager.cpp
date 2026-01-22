#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Boss3.h"  // ★追加★
#include "zako1.h"
#include "zako2.h"
#include "BackGround.h"
#include "Field.h"

EnemyManager::EnemyManager(BackGround* bg, int stageNumber)
    : GameObject(),
    pBackground(bg),
    currentStageData(nullptr),
    currentStageNumber(0),
    currentPhase(GamePhase::PHASE_1),
    currentPhaseIndex(0),
    currentWaveIndex(0),
    waveTimer(0.0f),
    waveSpawned(false),
    phaseTransitionTimer(0.0f),
    phaseCleared(false),
    m_gameStarted(false)
{
    LoadStageFromCSV(stageNumber);
}

EnemyManager::~EnemyManager() {
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();

    if (currentStageData) {
        delete currentStageData;
        currentStageData = nullptr;
    }
}

bool EnemyManager::LoadStageFromCSV(int stageNumber) {
    if (currentStageData) {
        delete currentStageData;
        currentStageData = nullptr;
    }

    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    delayedSpawns.clear();

    char filename[64];
    sprintf_s(filename, "stage%d.csv", stageNumber);

    currentStageData = new StageData();
    if (!currentStageData->LoadFromCSV(filename)) {
        delete currentStageData;

        switch (stageNumber) {
        case 1:
            currentStageData = new Stage1Data();
            break;
        case 2:
            currentStageData = new Stage2Data();
            break;
        case 3:  // ★追加★
            currentStageData = new Stage3Data();
            break;
        default:
            currentStageData = new Stage1Data();
            break;
        }

        currentStageData->Initialize();
    }

    currentStageNumber = stageNumber;

    currentPhase = GamePhase::PHASE_1;
    currentPhaseIndex = 0;
    currentWaveIndex = 0;
    waveSpawned = false;
    waveTimer = 0.0f;
    phaseCleared = false;
    phaseTransitionTimer = 0.0f;
    m_gameStarted = false;

    return true;
}

void EnemyManager::Update() {
    if (!pBackground || !currentStageData) return;

    if (!m_gameStarted) {
        for (auto* enemy : enemies) {
            if (enemy) enemy->Update();
        }
        RemoveDeadEnemies();
        return;
    }

    UpdatePhaseLogic();
    UpdateWaveSpawning();
    UpdateDelayedSpawns();

    for (auto* enemy : enemies) {
        if (enemy) enemy->Update();
    }

    RemoveDeadEnemies();
}

void EnemyManager::UpdatePhaseLogic() {
    if (currentPhase == GamePhase::PHASE_CLEAR) return;

    if (!phaseCleared && IsPhaseCleared()) {
        phaseCleared = true;
        phaseTransitionTimer = 0.0f;
    }

    if (phaseCleared) {
        float waitTime = (currentPhaseIndex < currentStageData->phases.size())
            ? currentStageData->phases[currentPhaseIndex].transitionWait
            : 2.0f;

        phaseTransitionTimer += 1.0f / 60.0f;

        if (phaseTransitionTimer >= waitTime) {
            AdvanceToNextPhase();
        }
    }
}

void EnemyManager::UpdateWaveSpawning() {
    if (phaseCleared || currentPhase == GamePhase::PHASE_CLEAR) return;
    if (currentPhaseIndex >= currentStageData->phases.size()) return;

    const PhaseData& phase = currentStageData->phases[currentPhaseIndex];

    if (currentWaveIndex < phase.waves.size()) {
        const WaveData& wave = phase.waves[currentWaveIndex];

        if (!waveSpawned) {
            waveTimer += 1.0f / 60.0f;

            if (waveTimer >= wave.waveDelay) {
                for (const auto& enemyData : wave.enemies) {
                    DelayedSpawn ds;
                    ds.data = enemyData;
                    ds.timer = 0.0f;
                    ds.spawned = false;
                    delayedSpawns.push_back(ds);
                }
                waveSpawned = true;

                printfDx("Wave %d spawned (Phase %d, Stage %d)\n",
                    currentWaveIndex + 1, currentPhaseIndex + 1, currentStageNumber);
            }
        }

        if (waveSpawned && IsCurrentWaveCleared()) {
            AdvanceToNextWave();
        }
    }
}

void EnemyManager::UpdateDelayedSpawns() {
    for (auto& ds : delayedSpawns) {
        if (!ds.spawned) {
            ds.timer += 1.0f / 60.0f;

            if (ds.timer >= ds.data.delayTime) {
                SpawnEnemy(ds.data);
                ds.spawned = true;
            }
        }
    }

    delayedSpawns.erase(
        std::remove_if(delayedSpawns.begin(), delayedSpawns.end(),
            [](const DelayedSpawn& ds) { return ds.spawned; }),
        delayedSpawns.end()
    );
}

void EnemyManager::SpawnEnemy(const EnemySpawnData& data) {
    Enemy* newEnemy = nullptr;

    switch (data.enemyType) {
    case 0:
        newEnemy = new zako1(data.spawnX, data.spawnY,
            static_cast<Zako1Pattern>(data.pattern));
        break;

    case 1:
        newEnemy = new Boss1(data.spawnX, data.spawnY);
        break;

    case 2:
        newEnemy = new zako2(data.spawnX, data.spawnY,
            static_cast<Zako2Pattern>(data.pattern));
        break;

    case 3:
        newEnemy = new Boss2(data.spawnX, data.spawnY);
        break;

    case 4:  // ★Boss3追加★
        newEnemy = new Boss3(data.spawnX, data.spawnY);
        break;

    default:
        printfDx("不明な敵タイプ: %d\n", data.enemyType);
        break;
    }

    if (newEnemy) {
        enemies.push_back(newEnemy);
        printfDx("敵生成: タイプ%d X=%.1f Y=%.1f パターン=%d\n",
            data.enemyType, data.spawnX, data.spawnY, data.pattern);
    }
}

bool EnemyManager::IsCurrentWaveCleared() const {
    return enemies.empty() && delayedSpawns.empty();
}

bool EnemyManager::IsPhaseCleared() const {
    if (currentPhaseIndex >= currentStageData->phases.size()) return true;

    const PhaseData& phase = currentStageData->phases[currentPhaseIndex];

    return (currentWaveIndex >= phase.waves.size())
        && enemies.empty()
        && delayedSpawns.empty();
}

void EnemyManager::AdvanceToNextWave() {
    currentWaveIndex++;
    waveSpawned = false;
    waveTimer = 0.0f;
}

void EnemyManager::AdvanceToNextPhase() {
    currentPhaseIndex++;

    if (currentPhaseIndex >= currentStageData->phases.size()) {
        currentPhase = GamePhase::PHASE_CLEAR;
    }
    else {
        if (currentPhaseIndex == 0) currentPhase = GamePhase::PHASE_1;
        else if (currentPhaseIndex == 1) currentPhase = GamePhase::PHASE_2;
        else if (currentPhaseIndex == 2) currentPhase = GamePhase::PHASE_3;
        else if (currentPhaseIndex == 3) currentPhase = GamePhase::PHASE_BOSS;
        else currentPhase = GamePhase::PHASE_CLEAR;
    }

    currentWaveIndex = 0;
    waveSpawned = false;
    waveTimer = 0.0f;
    phaseCleared = false;
    phaseTransitionTimer = 0.0f;

    printfDx("Advance to Phase %d (Stage %d)\n", currentPhaseIndex + 1, currentStageNumber);
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

void EnemyManager::DrawDebugInfo() {
    const char* phaseNames[] = { "PHASE 1", "PHASE 2", "PHASE 3", "BOSS", "CLEAR!" };
    int phaseIndex = static_cast<int>(currentPhase);

    if (!m_gameStarted) {
        DrawString(10, 40, "COUNTDOWN...", GetColor(255, 255, 0));
    }

    DrawFormatString(10, 60, GetColor(255, 255, 0),
        "Stage %d - %s", currentStageNumber, phaseNames[phaseIndex]);

    DrawFormatString(10, 80, GetColor(255, 255, 0),
        " ", enemies.size());

    if (currentPhaseIndex < currentStageData->phases.size()) {
        DrawFormatString(10, 100, GetColor(255, 255, 0),
            " ",
            currentWaveIndex + 1,
            currentStageData->phases[currentPhaseIndex].waves.size());
    }

    for (size_t i = 0; i < enemies.size() && i < 5; i++) {
        if (enemies[i]) {
            DrawFormatString(10, 120 + i * 20, GetColor(0, 255, 255),
                " ",
                i, enemies[i]->GetX(), enemies[i]->GetY(),
                enemies[i]->GetHP());
        }
    }

    if (phaseCleared && currentPhase != GamePhase::PHASE_CLEAR) {
        float waitTime = (currentPhaseIndex < currentStageData->phases.size())
            ? currentStageData->phases[currentPhaseIndex].transitionWait
            : 2.0f;

        DrawFormatString(10, 220, GetColor(0, 255, 0),
            "Phase Cleared! Next in %.1f sec...",
            waitTime - phaseTransitionTimer);
    }

    if (currentPhase == GamePhase::PHASE_CLEAR) {
        DrawFormatString(10, 240, GetColor(255, 255, 0),
            "=== STAGE %d CLEAR ===", currentStageNumber);
    }
}

void EnemyManager::Draw() {
    for (auto* enemy : enemies) {
        if (enemy) enemy->Draw();
    }
}

void EnemyManager::Reset() {
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    delayedSpawns.clear();

    currentPhase = GamePhase::PHASE_1;
    currentPhaseIndex = 0;
    currentWaveIndex = 0;
    waveSpawned = false;
    waveTimer = 0.0f;
    phaseCleared = false;
    phaseTransitionTimer = 0.0f;
    m_gameStarted = false;
}