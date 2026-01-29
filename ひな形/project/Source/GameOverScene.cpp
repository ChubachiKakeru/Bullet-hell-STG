#include "GameOverScene.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

GameOverScene::GameOverScene()
    : m_gameOverImage(-1)
    , m_timer(0)
{
    // TODO: 実際の画像パスに変更してください
    m_gameOverImage = LoadGraph("Graphics/GameOver.png");
}

GameOverScene::~GameOverScene()
{
    if (m_gameOverImage != -1)
    {
        DeleteGraph(m_gameOverImage);
    }
}

void GameOverScene::Update()
{
    m_timer++;

    // スペースキーでタイトルへ戻る
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::ChangeScene("TITLE");
    }
}

void GameOverScene::Draw()
{
    // 背景を暗くする
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

    // ゲームオーバー画像を表示
    if (m_gameOverImage != -1)
    {
        DrawGraph(320, 200, m_gameOverImage, TRUE);
    }
    else
    {
        SetFontSize(60);
        // デバッグ用テキスト表示
        DrawString(400, 300, "GAME OVER", GetColor(255, 0, 0));
        SetFontSize(20);
    }
    DrawString(450, 500, "SPACE: タイトルへ戻る", GetColor(255, 255, 255));
}