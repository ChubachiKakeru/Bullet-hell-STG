#include "StageSelectScene.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

StageSelectScene::StageSelectScene()
    : m_backgroundImage(-1)
    , m_selectedStage(0)
{
    m_backgroundImage = LoadGraph("Graphics/StageSelect.png");
}

StageSelectScene::~StageSelectScene()
{
    if (m_backgroundImage != -1)
    {
        DeleteGraph(m_backgroundImage);
    }
}

void StageSelectScene::Update()
{
    static int keyWait = 0;
    keyWait++;

    if (keyWait > 10)
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            m_selectedStage = (m_selectedStage - 1 + STAGE_COUNT) % STAGE_COUNT;
            keyWait = 0;
        }
        if (CheckHitKey(KEY_INPUT_DOWN))
        {
            m_selectedStage = (m_selectedStage + 1) % STAGE_COUNT;
            keyWait = 0;
        }
    }

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::ChangeScene("PLAY");
    }

    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
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

    DrawString(500, 100, "ステージ選択", GetColor(255, 255, 255));

    for (int i = 0; i < STAGE_COUNT; i++)
    {
        int color = (i == m_selectedStage) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawFormatString(550, 200 + i * 50, color, "ステージ %d", i + 1);
    }

    DrawString(450, 600, "SPACE: 決定 / ESC: タイトルへ", GetColor(255, 255, 255));
}