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
    // 画面サイズ（Screen.hで定義された1280x1280）
    const int screenWidth = 1280;
    const int screenHeight = 1280;

    // 背景を黒で塗りつぶし
    DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 0), TRUE);

    // タイトル画像を877:650の比率を保ったまま画面いっぱいに表示
    if (m_titleImage != -1)
    {
        // 画像の元サイズ（877x650）
        const int imageWidth = 877;
        const int imageHeight = 650;

        // 画面の高さ方向に合わせて拡大（877:650の比率を保持）
        // scale = 1280 / 650 = 1.969...
        float scale = (float)screenHeight / imageHeight;

        // 拡大後のサイズ
        // 幅: 877 * 1.969 = 1726.913 ≒ 1727
        // 高さ: 650 * 1.969 = 1280
        int drawWidth = (int)(imageWidth * scale);
        int drawHeight = screenHeight; // 画面の高さいっぱい

        // 中央配置の座標（幅が画面より大きいので左右が切れる）
        int drawX = (screenWidth - drawWidth) / 2;
        int drawY = 0; // 上端から表示

        // 描画（877:650の比率を維持したまま画面いっぱいに拡大）
        DrawExtendGraph(drawX, drawY, drawX + drawWidth, drawY + drawHeight, m_titleImage, TRUE);
    }
    else
    {
        // 画像がない場合のデバッグ表示
        DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 128), TRUE);
        DrawString(screenWidth / 2 - 50, screenHeight / 2 - 100, "GAME TITLE", GetColor(255, 255, 255));
    }

    // メニュー表示（画像の下部に重ねて表示）
    int textStartY = 850; // 画像の下部エリア

    // SPACE操作説明（縁取り付き白色テキスト）
    // 黒い縁取りで視認性向上
    DrawString(481, textStartY + 1, "SPACE: ゲーム開始", GetColor(0, 0, 0));
    DrawString(479, textStartY + 1, "SPACE: ゲーム開始", GetColor(0, 0, 0));
    DrawString(481, textStartY - 1, "SPACE: ゲーム開始", GetColor(0, 0, 0));
    DrawString(479, textStartY - 1, "SPACE: ゲーム開始", GetColor(0, 0, 0));
    DrawString(480, textStartY, "SPACE: ゲーム開始", GetColor(255, 255, 255));

    // デバッグメニュー表示（縁取り付き）
    // タイトル（黄色）
    DrawString(421, textStartY + 61, "=== デバッグメニュー ===", GetColor(0, 0, 0));
    DrawString(419, textStartY + 61, "=== デバッグメニュー ===", GetColor(0, 0, 0));
    DrawString(421, textStartY + 59, "=== デバッグメニュー ===", GetColor(0, 0, 0));
    DrawString(419, textStartY + 59, "=== デバッグメニュー ===", GetColor(0, 0, 0));
    DrawString(420, textStartY + 60, "=== デバッグメニュー ===", GetColor(255, 255, 0));

    // メニュー項目（シアン色で視認性向上）
    DrawString(381, textStartY + 96, "1: ステージ選択", GetColor(0, 0, 0));
    DrawString(379, textStartY + 96, "1: ステージ選択", GetColor(0, 0, 0));
    DrawString(381, textStartY + 94, "1: ステージ選択", GetColor(0, 0, 0));
    DrawString(379, textStartY + 94, "1: ステージ選択", GetColor(0, 0, 0));
    DrawString(380, textStartY + 95, "1: ステージ選択", GetColor(0, 255, 255));

    DrawString(381, textStartY + 126, "2: ショップ", GetColor(0, 0, 0));
    DrawString(379, textStartY + 126, "2: ショップ", GetColor(0, 0, 0));
    DrawString(381, textStartY + 124, "2: ショップ", GetColor(0, 0, 0));
    DrawString(379, textStartY + 124, "2: ショップ", GetColor(0, 0, 0));
    DrawString(380, textStartY + 125, "2: ショップ", GetColor(0, 255, 255));

    DrawString(381, textStartY + 156, "3: プレイシーン", GetColor(0, 0, 0));
    DrawString(379, textStartY + 156, "3: プレイシーン", GetColor(0, 0, 0));
    DrawString(381, textStartY + 154, "3: プレイシーン", GetColor(0, 0, 0));
    DrawString(379, textStartY + 154, "3: プレイシーン", GetColor(0, 0, 0));
    DrawString(380, textStartY + 155, "3: プレイシーン", GetColor(0, 255, 255));

    DrawString(381, textStartY + 186, "4: クリアシーン", GetColor(0, 0, 0));
    DrawString(379, textStartY + 186, "4: クリアシーン", GetColor(0, 0, 0));
    DrawString(381, textStartY + 184, "4: クリアシーン", GetColor(0, 0, 0));
    DrawString(379, textStartY + 184, "4: クリアシーン", GetColor(0, 0, 0));
    DrawString(380, textStartY + 185, "4: クリアシーン", GetColor(0, 255, 255));

    DrawString(381, textStartY + 216, "5: ゲームオーバー", GetColor(0, 0, 0));
    DrawString(379, textStartY + 216, "5: ゲームオーバー", GetColor(0, 0, 0));
    DrawString(381, textStartY + 214, "5: ゲームオーバー", GetColor(0, 0, 0));
    DrawString(379, textStartY + 214, "5: ゲームオーバー", GetColor(0, 0, 0));
    DrawString(380, textStartY + 215, "5: ゲームオーバー", GetColor(0, 255, 255));

    // デバッグシーン選択を最前面に描画
    DebugSceneSelector::Draw();
}
