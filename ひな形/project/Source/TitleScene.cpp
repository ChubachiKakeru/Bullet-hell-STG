#include "TitleScene.h"
#include "../Library/SceneManager.h"
#include"DebugSceneSelector.h"

TitleScene::TitleScene()
    : m_titleImage(-1)
{
    // タイトル画像を読み込む
    m_titleImage = LoadGraph("Graphics/Title.png");
}

TitleScene::~TitleScene()
{
    if (m_titleImage != -1)
    {
        DeleteGraph(m_titleImage);
    }
}

void TitleScene::Update()
{
    // デバッグシーン選択
    DebugSceneSelector::Update();

    // 通常の遷移
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::ChangeScene("STAGESELECT");
    }

    // デバッグ用：数字キーでシーン遷移
    if (CheckHitKey(KEY_INPUT_1))
    {
        SceneManager::ChangeScene("STAGESELECT");
    }
    if (CheckHitKey(KEY_INPUT_2))
    {
        SceneManager::ChangeScene("SHOP");
    }
    if (CheckHitKey(KEY_INPUT_3))
    {
        SceneManager::ChangeScene("PLAY");
    }
    if (CheckHitKey(KEY_INPUT_4))
    {
        SceneManager::ChangeScene("CLEAR");
    }
    if (CheckHitKey(KEY_INPUT_5))
    {
        SceneManager::ChangeScene("GAMEOVER");
    }
}

void TitleScene::Draw()
{
    // タイトル画像を表示
    if (m_titleImage != -1)
    {
        DrawGraph(0, 0, m_titleImage, FALSE);
    }
    else
    {
        // 画像がない場合のデバッグ表示
        DrawBox(0, 0, 1280, 720, GetColor(0, 0, 128), TRUE);
        DrawString(500, 200, "GAME TITLE", GetColor(255, 255, 255));
    }

    // メニュー表示
    DrawString(500, 350, "SPACE: ゲーム開始", GetColor(255, 255, 255));

    // デバッグメニュー表示
    DrawString(400, 450, "=== デバッグメニュー ===", GetColor(255, 255, 0));
    DrawString(450, 480, "1: ステージ選択", GetColor(200, 200, 200));
    DrawString(450, 510, "2: ショップ", GetColor(200, 200, 200));
    DrawString(450, 540, "3: プレイシーン", GetColor(200, 200, 200));
    DrawString(450, 570, "4: クリアシーン", GetColor(200, 200, 200));
    DrawString(450, 600, "5: ゲームオーバー", GetColor(200, 200, 200));

    // デバッグシーン選択を最前面に描画
    DebugSceneSelector::Draw();
}