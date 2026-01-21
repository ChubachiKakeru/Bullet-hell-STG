#include "EnemyManager.h"
#include "Enemy.h"
#include "Boss1.h"
#include "Boss2.h"
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
    m_gameStarted(false)  // ★追加: 初期状態はゲーム未開始★
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
    // 既存のステージデータを削除
    if (currentStageData) {
        delete currentStageData;
        currentStageData = nullptr;
    }

    // 敵を全て削除
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    delayedSpawns.clear();

    // CSVファイル名を生成（例: "stage1.csv", "stage2.csv"）
    char filename[64];
    sprintf_s(filename, "stage%d.csv", stageNumber);

    // CSVからデータを読み込み
    currentStageData = new StageData();
    if (!currentStageData->LoadFromCSV(filename)) {
        // CSVが無い場合はコード定義のデフォルトデータを使用
        delete currentStageData;

        switch (stageNumber) {
        case 1:
            currentStageData = new Stage1Data();
            break;
        case 2:
            currentStageData = new Stage2Data();
            break;
        default:
            currentStageData = new Stage1Data();
            break;
        }

        currentStageData->Initialize();
    }

    currentStageNumber = stageNumber;

    // 状態をリセット
    currentPhase = GamePhase::PHASE_1;
    currentPhaseIndex = 0;
    currentWaveIndex = 0;
    waveSpawned = false;
    waveTimer = 0.0f;
    phaseCleared = false;
    phaseTransitionTimer = 0.0f;
    m_gameStarted = false;  // ★ゲーム開始フラグもリセット★

    return true;
}

void EnemyManager::Update() {
    if (!pBackground || !currentStageData) return;

    // ★カウントダウン中は敵の出現処理をスキップ★
    if (!m_gameStarted) {
        // ゲーム開始前は何もしない
        // （既に出現している敵がいれば更新するが、通常はいないはず）
        for (auto* enemy : enemies) {
            if (enemy) enemy->Update();
        }
        RemoveDeadEnemies();
        return;
    }

    // ★ゲーム開始後の通常処理★
    UpdatePhaseLogic();
    UpdateWaveSpawning();
    UpdateDelayedSpawns();

    // 全敵の更新
    for (auto* enemy : enemies) {
        if (enemy) enemy->Update();
    }

    RemoveDeadEnemies();
    //DrawDebugInfo();
}

void EnemyManager::UpdatePhaseLogic() {
    if (currentPhase == GamePhase::PHASE_CLEAR) return;

    // フェーズクリア判定
    if (!phaseCleared && IsPhaseCleared()) {
        phaseCleared = true;
        phaseTransitionTimer = 0.0f;
    }

    // フェーズクリア後の遷移待機
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

    // 現在のウェーブがまだある場合
    if (currentWaveIndex < phase.waves.size()) {
        const WaveData& wave = phase.waves[currentWaveIndex];

        if (!waveSpawned) {
            waveTimer += 1.0f / 60.0f;

            // ウェーブの遅延時間経過後に出現
            if (waveTimer >= wave.waveDelay) {
                // ウェーブ内の全敵を遅延スポーン登録
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

        // 現在のウェーブの敵が全滅したら次のウェーブへ
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

    // 出現済みのものを削除
    delayedSpawns.erase(
        std::remove_if(delayedSpawns.begin(), delayedSpawns.end(),
            [](const DelayedSpawn& ds) { return ds.spawned; }),
        delayedSpawns.end()
    );
}

void EnemyManager::SpawnEnemy(const EnemySpawnData& data) {
    Enemy* newEnemy = nullptr;

    switch (data.enemyType) {
    case 0:  // zako1（ステージ1用）
        newEnemy = new zako1(data.spawnX, data.spawnY,
            static_cast<Zako1Pattern>(data.pattern));
        break;

    case 1:  // Boss1
        newEnemy = new Boss1(data.spawnX, data.spawnY);
        break;

    case 2:  // zako2（ステージ2用）
        newEnemy = new zako2(data.spawnX, data.spawnY,
            static_cast<Zako2Pattern>(data.pattern));
        break;

    case 3:  // Boss2（ステージ2ボス）
        newEnemy = new Boss2(data.spawnX, data.spawnY);
        break;

        // 将来の拡張用
        // case 4:  // zako3
        //     newEnemy = new zako3(data.spawnX, data.spawnY, data.pattern);
        //     break;

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

    // 全ウェーブが完了 && 全敵撃破
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

    // フェーズ番号の更新
    if (currentPhaseIndex >= currentStageData->phases.size()) {
        currentPhase = GamePhase::PHASE_CLEAR;
    }
    else {
        // フェーズ番号を更新（PHASE_1, PHASE_2, ...）
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

    // ★ゲーム開始状態を表示★
    if (!m_gameStarted) {
        DrawString(10, 40, "COUNTDOWN...", GetColor(255, 255, 0));
    }

    // ステージとフェーズ表示
    DrawFormatString(10, 60, GetColor(255, 255, 0),
        "Stage %d - %s", currentStageNumber, phaseNames[phaseIndex]);

    DrawFormatString(10, 80, GetColor(255, 255, 0),
        " ", enemies.size()); //Active Enemies : % d

    if (currentPhaseIndex < currentStageData->phases.size()) {
        DrawFormatString(10, 100, GetColor(255, 255, 0),
            " ",
            currentWaveIndex + 1,
            currentStageData->phases[currentPhaseIndex].waves.size()); //Wave: %d/%d
    }

    // 各敵の座標表示（最大5体）
    for (size_t i = 0; i < enemies.size() && i < 5; i++) {
        if (enemies[i]) {
            DrawFormatString(10, 120 + i * 20, GetColor(0, 255, 255),
                " ",
                i, enemies[i]->GetX(), enemies[i]->GetY(),
                enemies[i]->GetHP()); //Enemy[%d]: X=%.1f, Y=%.1f, HP=%d
        }
    }

    // フェーズクリア待機表示
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
    m_gameStarted = false;  // ★リセット時もゲーム未開始に戻す★
}