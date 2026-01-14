#pragma once
#include "../Library/SceneBase.h"
#include "StageData.h"

class StageSelectScene : public SceneBase
{
public:
    StageSelectScene();
    virtual ~StageSelectScene();

    virtual void Update() override;
    virtual void Draw() override;

    // 選択されたステージ番号を取得（1～2）
    static int GetSelectedStage() { return s_selectedStageNumber; }

    // 選択されたステージデータを取得
    static StageData* GetCurrentStageData() { return s_currentStageData; }

private:
    void LoadStageData(int stageNumber);

    int m_backgroundImage;
    int m_selectedStage; // 0-indexed (0～1)
    int m_keyWait;

    static const int STAGE_COUNT = 2; // ★2ステージに変更★

    static int s_selectedStageNumber;
    static Stage1Data s_stage1;
    static Stage2Data s_stage2;
    static StageData* s_currentStageData;
};