// StageSelectScene.cpp
#include "StageSelectScene.h"
#include "../Library/SceneManager.h"
#include "DxLib.h"

// 静的メンバの初期化
int StageSelectScene::s_selectedStageNumber = 1;
Stage1Data StageSelectScene::s_stage1;
Stage2Data StageSelectScene::s_stage2;
Stage3Data StageSelectScene::s_stage3;  // ★追加★
StageData* StageSelectScene::s_currentStageData = nullptr;
int StageSelectScene::s_totalGameTimer = 0;

StageSelectScene::StageSelectScene()
    : m_backgroundImage(-1)
    , m_selectedStage(0)
    , m_keyWait(0)
{
    m_backgroundImage = LoadGraph("Graphics/StageSelect.png");
    m_selectedStage = s_selectedStageNumber - 1;
    m_keyWait = -60;

    CancelSoundHandle = LoadSoundMem(GAME_CANCEL_SOUND_PATH);
    CusorSoundHandle = LoadSoundMem(GAME_CURSOR_SOUND_PATH);
    DecisionSoundHandle = LoadSoundMem(GAME_DECISION_SOUND_PATH);
}

StageSelectScene::~StageSelectScene()
{
    if (m_backgroundImage != -1)
    {
        DeleteGraph(m_backgroundImage);
    }

    DeleteSoundMem(CancelSoundHandle);
    DeleteSoundMem(CusorSoundHandle);
}

void StageSelectScene::LoadStageData(int stageNumber)
{
    switch (stageNumber)
    {
    case 1:
        if (!s_stage1.LoadFromCSV("data/stage1.csv"))
        {
            printfDx("Stage1 CSV not found, using hardcoded data\n");
            s_stage1.Initialize();
        }
        s_currentStageData = &s_stage1;
        break;

    case 2:
        if (!s_stage2.LoadFromCSV("data/stage2.csv"))
        {
            printfDx("Stage2 CSV not found, using hardcoded data\n");
            s_stage2.Initialize();
        }
        s_currentStageData = &s_stage2;
        break;

    case 3:  // ★追加★
        if (!s_stage3.LoadFromCSV("data/stage3.csv"))
        {
            printfDx("Stage3 CSV not found, using hardcoded data\n");
            s_stage3.Initialize();
        }
        s_currentStageData = &s_stage3;
        break;

    default:
        printfDx("Invalid stage number: %d\n", stageNumber);
        s_currentStageData = nullptr;
        break;
    }
}

void StageSelectScene::GoToNextStage()
{
    int previousStage = s_selectedStageNumber;

    // ★ステージ1クリア後はステージ番号を2に更新してショップへ★
    if (previousStage == 1)
    {
        s_selectedStageNumber = 2;
        LoadStageData(s_selectedStageNumber);
        SceneManager::ChangeScene("SHOP");
        return;
    }

    // ★ステージ2クリア後はクリア画面へ（番号は更新しない）★
    if (previousStage == 2)
    {
        SceneManager::ChangeScene("CLEAR");
        return;
    }

    // ★ステージ3（隠しステージ）クリア後もクリア画面へ★
    if (previousStage == 3)
    {
        SceneManager::ChangeScene("CLEAR");
        return;
    }

    // デフォルト（念のため）
    SceneManager::ChangeScene("CLEAR");
}

void StageSelectScene::Update()
{
    m_keyWait++;

    if (m_keyWait < 0)
    {
        return;
    }

    if (m_keyWait > 10)
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            m_selectedStage = (m_selectedStage - 1 + STAGE_COUNT) % STAGE_COUNT;
            m_keyWait = 0;
            PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            m_selectedStage = (m_selectedStage + 1) % STAGE_COUNT;
            m_keyWait = 0;
            PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
        }
    }

    if (m_keyWait > 10 && (CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_RETURN)))
    {
        PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);

        s_selectedStageNumber = m_selectedStage + 1;

        if (s_selectedStageNumber == 1)
        {
            s_totalGameTimer = 0;
        }

        LoadStageData(s_selectedStageNumber);
        PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);

        SceneManager::ChangeScene("PLAY");
    }

    if (CheckHitKey(KEY_INPUT_O))
    {
        PlaySoundMem(CancelSoundHandle, DX_PLAYTYPE_BACK);
        SceneManager::ChangeScene("TITLE");
    }
}

void StageSelectScene::Draw()
{
    if (m_backgroundImage != -1)
    {
        DrawGraph(0, 0, m_backgroundImage, FALSE);
    }
    else
    {
        DrawBox(0, 0, 1280, 720, GetColor(50, 50, 100), TRUE);
    }

    SetFontSize(48);
    DrawString(450, 100, "=== ステージ選択 ===", GetColor(255, 255, 255));

    SetFontSize(36);
    for (int i = 0; i < STAGE_COUNT; i++)
    {
        int yPos = 300 + i * 80;

        if (i == m_selectedStage)
        {
            DrawString(480, yPos, "＞", GetColor(255, 255, 0));
            DrawBox(500, yPos - 5, 780, yPos + 45, GetColor(255, 255, 0), FALSE);
            DrawFormatString(520, yPos, GetColor(255, 255, 0), "ステージ %d", i + 1);
        }
        else
        {
            DrawFormatString(520, yPos, GetColor(180, 180, 180), "ステージ %d", i + 1);
        }
    }

    SetFontSize(24);
    DrawString(400, 620, "↑↓キーで選択 / SPACE/ENTER: 決定 / O: タイトルへ",
        GetColor(200, 200, 200));
    SetFontSize(16);
}