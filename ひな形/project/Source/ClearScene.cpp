#include "ClearScene.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

ClearScene::ClearScene()
    : m_clearImage(-1)
    , m_timer(0)
{
    m_clearImage = LoadGraph("Graphics/Clear.png");
}

ClearScene::~ClearScene()
{
    if (m_clearImage != -1)
    {
        DeleteGraph(m_clearImage);
    }
}

void ClearScene::Update()
{
    m_timer++;

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::ChangeScene("STAGESELECT");
    }

    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        SceneManager::ChangeScene("TITLE");
    }
}

void ClearScene::Draw()
{
    DrawBox(0, 0, 1280, 720, GetColor(200, 220, 255), TRUE);

    if (m_clearImage != -1)
    {
        DrawGraph(320, 200, m_clearImage, TRUE);
    }
    else
    {
        DrawString(500, 300, "STAGE CLEAR!", GetColor(0, 200, 0));
    }

    DrawString(400, 500, "SPACE: ステージ選択へ", GetColor(0, 0, 0));
    DrawString(400, 530, "ESC: タイトルへ戻る", GetColor(0, 0, 0));
}