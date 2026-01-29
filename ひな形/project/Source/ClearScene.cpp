#include "ClearScene.h"
#include "../Library/SceneManager.h"
#include "StageSelectScene.h"

ClearScene::ClearScene()
    : m_clearImage(-1)
    , m_timer(0)
{
    m_clearImage = LoadGraph("data/image/Clear.jpg");
    // ★ステージ1開始からの累計時間を取得★
    m_timer = StageSelectScene::s_totalGameTimer;
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
    // ★m_timer++を削除（カウントアップしない）★
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
    DrawBox(0, 0, 1280, 1280, GetColor(200, 220, 255), TRUE);

    if (m_clearImage != -1)
    {
        // 画像を画面いっぱいに拡大表示（800x800 → 1280x1280）
        DrawExtendGraph(0, 0, 1280, 1280, m_clearImage, TRUE);
    }
    else
    {
        DrawString(500, 600, "STAGE CLEAR!", GetColor(0, 0, 0));
    }

    // クリアタイム表示（ステージ1開始からの累計時間）
    int clearTimeSeconds = m_timer / 60; // 60FPSを想定
    int minutes = clearTimeSeconds / 60;
    int seconds = clearTimeSeconds % 60;
    DrawFormatString(150, 300, GetColor(0, 0, 0), "クリアタイム: %02d:%02d", minutes, seconds);

    DrawString(150, 350, "SPACE: ステージ選択へ", GetColor(0, 0, 0));
    DrawString(160, 400, "ESC: タイトルへ戻る", GetColor(0, 0, 0));
    SetFontSize(35);
    DrawString(50, 600, "「助けてくれてありがとう、\n"
        "  私の可愛い人形さん」", GetColor(0, 0, 0));
    SetFontSize(20);
}