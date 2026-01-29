#include "ShopScene.h"
#include "Player.h"
#include"Bomb.h"
#include "../Library/SceneManager.h"
#include "StageSelectScene.h"


enum ShopState
{
    //    SHOP_SELECT,
    SHOP_CONFIRM
};

ShopScene::ShopScene()
    : m_backgroundImage(-1), m_selectedItem(0)
    , m_isConfirm(false), m_yesNoSelect(0)
{
    m_backgroundImage = LoadGraph("data/image/shop.jpg");
}

ShopScene::~ShopScene()
{
    if (m_backgroundImage != -1)
    {
        DeleteGraph(m_backgroundImage);
    }
}

void ShopScene::Update()
{
    static bool prevPush = false;
    static int keyWait = 0;
    keyWait++;

    if (!m_isConfirm)
    {
        if (keyWait > 10)
        {
            if (CheckHitKey(KEY_INPUT_UP))
            {
                m_selectedItem = (m_selectedItem - 1 + ITEM_COUNT) % ITEM_COUNT;
                keyWait = 0;
            }
            if (CheckHitKey(KEY_INPUT_DOWN))
            {
                m_selectedItem = (m_selectedItem + 1) % ITEM_COUNT;
                keyWait = 0;
            }
        }
        if (!m_isConfirm)
        {
            if (CheckHitKey(KEY_INPUT_RETURN) && !prevPush)
            {
                m_isConfirm = true;
                m_yesNoSelect = 0;
                keyWait = 0;
                prevPush = true;
            }
        }



        if (CheckHitKey(KEY_INPUT_RETURN))
        {
            m_isConfirm = true;
            m_yesNoSelect = 0; // 「はい」を初期選択
            m_yesNoSelect = false;
            m_yesNoSelect = 1;
            keyWait = 0;
        }



        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            // ステージ2へ遷移（既にs_selectedStageNumberは2なのでそのままPLAYへ）
            SceneManager::ChangeScene("PLAY");
        }

    }
    else
    {
        if (keyWait > 10)
        {
            if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_RIGHT))
            {
                m_yesNoSelect = 1 - m_yesNoSelect;
                keyWait = 0;
            }
        }
    }
            // ★ ここに書く ★
            if (CheckHitKey(KEY_INPUT_RETURN) && !prevPush)
            {
                if (m_yesNoSelect == 0)
                {
                    switch (m_selectedItem)
                    {
                    case 0:
                        Player::UpgradeMaxHP(1);
                        break;
                    case 1:
                        Player::UpgradeInitialBombCount(5);
                        break;
                    }
                }



                m_isConfirm = false; // ダイアログを閉じる
                keyWait = 0;
                prevPush = true;
            }
    
    if (!!CheckHitKey(KEY_INPUT_RETURN))
    {
        prevPush = false;
    }
}

void ShopScene::Draw()
{
    // 画面サイズ
    const int screenWidth = 1280;
    const int screenHeight = 1280;

    if (m_backgroundImage != -1)
    {
        // 画像の元サイズ（877x620）
        const int imageWidth = 877;
        const int imageHeight = 620;

        // ★画面の高さ方向に合わせて拡大（877:620の比率を保持）★
        // scale = 1280 / 620 = 2.064...
        float scale = (float)screenHeight / imageHeight;

        // 拡大後のサイズ
        // 幅: 877 * 2.064 = 1810.128 ≒ 1810
        // 高さ: 620 * 2.064 = 1280
        int drawWidth = (int)(imageWidth * scale);
        int drawHeight = screenHeight; // 画面の高さいっぱい

        // 中央配置の座標（幅が画面より大きいので左右が切れる）
        int drawX = (screenWidth - drawWidth) / 2;
        int drawY = 0; // 上端から表示

        // 描画（877:620の比率を維持したまま画面いっぱいに拡大）
        DrawExtendGraph(drawX, drawY, drawX + drawWidth, drawY + drawHeight, m_backgroundImage, TRUE);

    }
    else
    {
        DrawBox(0, 0, screenWidth, screenHeight, GetColor(100, 50, 50), TRUE);
    }

    // ★デバッグ表示★
    DrawFormatString(10, 10, GetColor(255, 255, 0), "現在のステージ番号: %d", StageSelectScene::GetSelectedStageNumber());
    SetFontSize(48);
    DrawString(533, 200, "ショップ", GetColor(25, 255, 255));
    SetFontSize(24);
    DrawFormatString(10, 50, GetColor(255, 255, 255), "現在の最大HP: %d", Player::GetStaticMaxHP());
    DrawFormatString(10, 80, GetColor(255, 255, 255), "現在の初期ボム数: %d", Player::GetStaticInitialBombCount());

    const char* items[] = {
        "体力+1",
        "Bome+5",
        //  "攻撃速度UP",
        // "Bome範囲",
    };

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        int color = (i == m_selectedItem) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawString(533, 400 + i * 50, items[i], color);
    }
    SetFontSize(24);
    DrawString(400, 600, "↑↓キーで選択/ ENTER:決定 / ESC: ステージ2へ", GetColor(255, 255, 255));

    if (m_isConfirm)
    {
        DrawBox(340, 250, 940, 420, GetColor(0, 0, 0), TRUE);
        DrawBox(340, 250, 940, 420, GetColor(255, 255, 255), FALSE);
        DrawString(520, 300, "このアイテムを購入しますか？", GetColor(255, 255, 255));

        int yesColor = (m_yesNoSelect == 0) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        int noColor = (m_yesNoSelect == 1) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

        DrawString(550, 360, "はい", yesColor);
        DrawString(650, 360, "いいえ", noColor);
    }
}