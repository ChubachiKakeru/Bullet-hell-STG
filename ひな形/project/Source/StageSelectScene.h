#pragma once
#include "../Library/SceneBase.h"
#include "StageData.h" // Stage1Data, Stage2Data, Stage3Data, StageDataの定義を含むヘッダー

class StageSelectScene : public SceneBase
{
public:
    StageSelectScene();
    virtual ~StageSelectScene();

    virtual void Update() override;
    virtual void Draw() override;

    // ステージデータを読み込む
    static void LoadStageData(int stageNumber);

    // 次のステージに進む（ステージクリア時に呼ぶ）
    static void GoToNextStage();

    // 現在選択されているステージ番号を取得
    static int GetSelectedStageNumber() { return s_selectedStageNumber; }

    // 現在のステージデータを取得
    static StageData* GetCurrentStageData() { return s_currentStageData; }

    // ★タイマーリセット（publicに移動）★
    static void ResetTotalTimer() { s_totalGameTimer = 0; }

    // ★静的メンバ変数（全インスタンスで共有）★
    static int s_selectedStageNumber;       // 選択されたステージ番号（1-indexed）
    static Stage1Data s_stage1;             // ステージ1のデータ
    static Stage2Data s_stage2;             // ステージ2のデータ
    static Stage3Data s_stage3;             // ★ステージ3のデータ（追加）★
    static StageData* s_currentStageData;   // 現在のステージデータへのポインタ
    static int s_totalGameTimer;            // ★ゲーム全体の経過時間（publicに移動）★

private:
    static const int STAGE_COUNT = 2;       // ★通常ステージ数は2（ステージ3は隠しステージ）★

    int m_backgroundImage;                  // 背景画像
    int m_selectedStage;                    // 選択中のステージ（0-indexed）
    int m_keyWait;                          // キー入力待機時間

    // サウンドパス
    const char* GAME_CANCEL_SOUND_PATH = "data/Sound/cancel.mp3";      //キャンセルジングル
    const char* GAME_CURSOR_SOUND_PATH = "data/Sound/cursor.mp3";      //選択ジングル
    const char* GAME_DECISION_SOUND_PATH = "data/Sound/decision.mp3";  //決定ジングル

    //  サウンドハンドル
    int CancelSoundHandle;      // キャンセルサウンドハンドル
    int CusorSoundHandle;       // 選択サウンドハンドル
    int DecisionSoundHandle;    // 決定サウンドハンドル
};