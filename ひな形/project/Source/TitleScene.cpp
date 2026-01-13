#include "TitleScene.h"
#include "../Library/SceneManager.h"
#include "DebugSceneSelector.h"

TitleScene::TitleScene()
    : m_titleImage(-1)
{
    // タイトル画像を読み込む
    m_titleImage = LoadGraph("data/image/title.png");
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
    // 背景を黒で塗りつぶし
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

    // タイトル画像をアスペクト比を保って中央に表示
    if (m_titleImage != -1)
    {
        // 画像のサイズを取得
        int imageWidth, imageHeight;
        GetGraphSize(m_titleImage, &imageWidth, &imageHeight);

        // 画面サイズ
        const int screenWidth = 1280;
        const int screenHeight = 720;

        // アスペクト比を保って拡大率を計算
        float scaleX = (float)screenWidth / imageWidth;
        float scaleY = (float)screenHeight / imageHeight;
        float scale = (scaleX < scaleY) ? scaleX : scaleY; // 小さい方を採用

        // 拡大後のサイズ
        int drawWidth = (int)(imageWidth * scale);
        int drawHeight = (int)(imageHeight * scale);

        // 中央配置の座標
        int drawX = (screenWidth - drawWidth) / 2;
        int drawY = (screenHeight - drawHeight) / 2;

        // 描画
        DrawExtendGraph(drawX, drawY, drawX + drawWidth, drawY + drawHeight, m_titleImage, FALSE);
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