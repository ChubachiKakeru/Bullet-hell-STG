#pragma once
#include "zako1.h"
#include <vector>
#include <string>

// 敵の出現データ
struct EnemySpawnData {
    int enemyType;          // 0=zako1, 1=Boss1, 2=zako2, など
    float spawnX;           // 出現X座標
    float spawnY;           // 出現Y座標
    int pattern;            // 行動パターン
    float delayTime;        // このウェーブ開始からの遅延時間（秒）
};

// ウェーブデータ（敵のグループ）
struct WaveData {
    std::vector<EnemySpawnData> enemies;  // このウェーブで出現する敵リスト
    float waveDelay;                      // 前のウェーブからの待機時間（秒）
};

// フェーズデータ
struct PhaseData {
    std::vector<WaveData> waves;          // このフェーズのウェーブリスト
    float transitionWait;                 // フェーズクリア後の待機時間（秒）
};

// ステージ全体のデータ
class StageData {
public:
    std::vector<PhaseData> phases;        // ステージの全フェーズ

    virtual ~StageData() = default;

    // CSVファイルからデータを読み込む
    bool LoadFromCSV(const std::string& filename);

    // コードで直接定義する場合（プログラマー・上級者向け）
    virtual void Initialize() {}

private:
    void ParseCSVData(class CsvReader& csv);
};

// ========================================
// コードで定義するステージ（プログラマー向け）
// ========================================

// ステージ1のデータ（コード定義版）
class Stage1Data : public StageData {
public:
    void Initialize() override;
};

// ステージ2のデータ（コード定義版・例）
class Stage2Data : public StageData {
public:
    void Initialize() override;
};