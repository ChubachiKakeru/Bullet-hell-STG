#include "StageData.h"
#include "csvReader.h"
#include "Field.h"


// ========================================
// CSV読み込み処理
// ========================================
bool StageData::LoadFromCSV(const std::string& filename) {
    CsvReader csv(filename);

    if (csv.GetLines() == 0) {
        printfDx("CSVファイルの読み込みに失敗: %s\n", filename.c_str());
        return false;
    }

    ParseCSVData(csv);

    printfDx("CSVファイル読み込み完了: %s (%d phases)\n",
        filename.c_str(), phases.size());
    return true;
}

void StageData::ParseCSVData(CsvReader& csv) {
    phases.clear();

    int currentPhaseNum = -1;
    int currentWaveNum = -1;
    PhaseData* currentPhase = nullptr;
    WaveData* currentWave = nullptr;

    // 1行目はヘッダーなのでスキップして2行目から読む
    for (int line = 1; line < csv.GetLines(); line++) {
        // 空行はスキップ
        if (csv.GetColumns(line) < 8) continue;

        int phaseNum = csv.GetInt(line, 0);
        int waveNum = csv.GetInt(line, 1);

        // 新しいフェーズの開始
        if (phaseNum != currentPhaseNum) {
            phases.push_back(PhaseData());
            currentPhase = &phases.back();
            currentPhase->transitionWait = csv.GetFloat(line, 7);
            currentPhaseNum = phaseNum;
            currentWaveNum = -1;
        }

        // 新しいウェーブの開始
        if (waveNum != currentWaveNum && currentPhase) {
            currentPhase->waves.push_back(WaveData());
            currentWave = &currentPhase->waves.back();
            currentWave->waveDelay = csv.GetFloat(line, 2);
            currentWaveNum = waveNum;
        }

        // 敵データを追加
        if (currentWave) {
            EnemySpawnData enemy;
            enemy.enemyType = csv.GetInt(line, 3);
            enemy.spawnX = csv.GetFloat(line, 4);
            enemy.spawnY = csv.GetFloat(line, 5);
            enemy.pattern = csv.GetInt(line, 6);
            enemy.delayTime = csv.GetFloat(line, 8);  // 個別遅延時間

            currentWave->enemies.push_back(enemy);
        }
    }
}

// ========================================
// ステージ1のデータ（コード定義版・CSVが無い場合のフォールバック）
// ========================================
void Stage1Data::Initialize() {
    phases.clear();
    phases.resize(4);  // 4フェーズ分

    // ========================================
    // フェーズ1: 左から右へ移動する敵 5体
    // ========================================
    {
        PhaseData& phase = phases[0];
        phase.transitionWait = 2.0f;  // フェーズクリア後2秒待機

        // ウェーブ1: 5体同時出現
        WaveData wave;
        wave.waveDelay = 0.0f;  // フェーズ開始と同時

        // 旧コード（参考用）:
        // phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + -100.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
        // phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 50.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
        // phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 200.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
        // phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 350.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });
        // phaseSpawnData[0].push_back({ 0, Field::STAGE_LEFT + 500.0f, Field::STAGE_TOP + 50.0f, Zako1Pattern::PATTERN_LEFT_TO_RIGHT });

        wave.enemies.push_back({ 0, Field::STAGE_LEFT + -100.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 50.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 200.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 350.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 500.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });

        phase.waves.push_back(wave);
    }

    // ========================================
    // フェーズ2: 右から左へ移動する敵 5体
    // ========================================
    {
        PhaseData& phase = phases[1];
        phase.transitionWait = 2.0f;

        // ウェーブ1: 5体同時出現
        WaveData wave;
        wave.waveDelay = 0.0f;

        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 150.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 300.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 450.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 600.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave.enemies.push_back({ 0, Field::STAGE_LEFT + 750.0f, Field::STAGE_TOP + 50.0f,
                                static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });

        phase.waves.push_back(wave);
    }

    // ========================================
    // フェーズ3: 2つのウェーブ
    // 第1波: 左から5体
    // 第2波: 2秒後に右から3体
    // ========================================
    {
        PhaseData& phase = phases[2];
        phase.transitionWait = 2.0f;

        // ウェーブ1: 左から5体
        WaveData wave1;
        wave1.waveDelay = 0.0f;

        wave1.enemies.push_back({ 0, Field::STAGE_LEFT, Field::STAGE_TOP + 50.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave1.enemies.push_back({ 0, Field::STAGE_LEFT + 150.0f, Field::STAGE_TOP + 50.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave1.enemies.push_back({ 0, Field::STAGE_LEFT + 300.0f, Field::STAGE_TOP + 50.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave1.enemies.push_back({ 0, Field::STAGE_LEFT + 450.0f, Field::STAGE_TOP + 50.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave1.enemies.push_back({ 0, Field::STAGE_LEFT + 600.0f, Field::STAGE_TOP + 50.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });

        // ウェーブ2: 2秒後に右から3体
        // WaveData wave2;
        //wave2.waveDelay = 2.0f;  // 第1波から2秒後

        float rightStart = Field::STAGE_RIGHT - 100.0f;
        wave1.enemies.push_back({ 0, rightStart - 200.0f, Field::STAGE_TOP + 70.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave1.enemies.push_back({ 0, rightStart + 0.0f, Field::STAGE_TOP + 70.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave1.enemies.push_back({ 0, rightStart + 200.0f, Field::STAGE_TOP + 70.0f,
                                 static_cast<int>(Zako1Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });

        phase.waves.push_back(wave1);
    }

    // ========================================
    // フェーズ4: ボス戦
    // ========================================
    {
        PhaseData& phase = phases[3];
        phase.transitionWait = 2.0f;

        // ウェーブ1: ボス1体
        WaveData wave;
        wave.waveDelay = 0.0f;

        float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;
        wave.enemies.push_back({ 1, centerX - 60.0f, Field::STAGE_TOP + 20.0f, 0, 0.0f });

        phase.waves.push_back(wave);
    }
}

// ========================================
// ステージ2のデータ（コード定義版・例）
// ========================================
void Stage2Data::Initialize() {
    phases.clear();
    phases.resize(3);  // フェーズ1～3

    // ========================================
    // フェーズ1: 上から下に降りてくる敵の列 3体
    // ========================================
    {
        PhaseData& phase = phases[0];
        phase.transitionWait = 1.5f;

        WaveData wave;
        wave.waveDelay = 0.0f;

        // 4列の敵
        float sx = Field::STAGE_LEFT - 60.0f;
        float sy = Field::STAGE_TOP - 10.0f;
        wave.enemies.push_back({ 2, Field::STAGE_LEFT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave.enemies.push_back({ 2, Field::STAGE_LEFT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT), 0.6f });
        wave.enemies.push_back({ 2, Field::STAGE_LEFT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT), 1.2f });
        wave.enemies.push_back({ 2, Field::STAGE_LEFT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT),  1.8f });

        phase.waves.push_back(wave);
    }

    // ========================================
    // フェーズ2: 複数ウェーブの連続攻撃
    // ========================================
    {
        PhaseData& phase = phases[1];
        phase.transitionWait = 2.0f;

        // 第1波: 左から3体（★zako2使用★）
        WaveData wave1;
        wave1.waveDelay = 0.0f;
        float sx = Field::STAGE_RIGHT + 60.0f;
        float sy = Field::STAGE_TOP - 10.0f;
        wave1.enemies.push_back({ 2, Field::STAGE_RIGHT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave1.enemies.push_back({ 2, Field::STAGE_RIGHT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.6f });
        wave1.enemies.push_back({ 2, Field::STAGE_RIGHT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 1.2f });
        wave1.enemies.push_back({ 2, Field::STAGE_RIGHT , Field::STAGE_TOP,
                                static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT),  1.8f });

        phase.waves.push_back(wave1);

        // 第2波: 1秒後に右から3体（★zako2使用★）
        WaveData wave2;
        wave2.waveDelay = 1.0f;
        wave2.enemies.push_back({ 2, Field::STAGE_RIGHT - 100.0f, Field::STAGE_TOP + 150.0f,
                                 static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave2.enemies.push_back({ 2, Field::STAGE_RIGHT - 250.0f, Field::STAGE_TOP + 150.0f,
                                 static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave2.enemies.push_back({ 2, Field::STAGE_RIGHT - 400.0f, Field::STAGE_TOP + 150.0f,
                                 static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        phase.waves.push_back(wave2);

        // 第3波: さらに1秒後に中央から4体（★zako2使用★）
        WaveData wave3;
        wave3.waveDelay = 1.0f;
        float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;
        wave3.enemies.push_back({ 2, centerX - 150.0f, Field::STAGE_TOP,
                                 static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave3.enemies.push_back({ 2, centerX - 50.0f, Field::STAGE_TOP,
                                 static_cast<int>(Zako2Pattern::PATTERN_LEFT_TO_RIGHT), 0.0f });
        wave3.enemies.push_back({ 2, centerX + 50.0f, Field::STAGE_TOP,
                                 static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        wave3.enemies.push_back({ 2, centerX + 150.0f, Field::STAGE_TOP,
                                 static_cast<int>(Zako2Pattern::PATTERN_RIGHT_TO_LEFT), 0.0f });
        phase.waves.push_back(wave3);
    }

    // ========================================
    // フェーズ3: ボス戦
    // ========================================
    {
        PhaseData& phase = phases[2];
        phase.transitionWait = 3.0f;

        WaveData wave;
        wave.waveDelay = 0.0f;

        float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;

        // ★Boss2が実装されている場合: enemyType = 3★
         wave.enemies.push_back({ 3, centerX - 60.0f, Field::STAGE_TOP + 20.0f, 0, 0.0f });

        // ★Boss2が未実装の場合: 一時的にBoss1を使用★
        //wave.enemies.push_back({ 1, centerX - 60.0f, Field::STAGE_TOP + 20.0f, 0, 0.0f });

        phase.waves.push_back(wave);
    }
}