#include "ShopScene.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

enum ShopState
{
//    SHOP_SELECT,
   SHOP_CONFIRM
};
ShopScene::ShopScene()
    : m_backgroundImage(-1) , m_selectedItem(0)
    , m_isConfirm(false) , m_yesNoSelect(0)
{
    m_backgroundImage = LoadGraph("Graphics/Shop.png");
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

        if (CheckHitKey(KEY_INPUT_RETURN))
        {
            m_selectedItem = true;
            m_selectedItem = 0; // 「はい」を初期選択
            keyWait = 0;
        }

        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            SceneManager::ChangeScene("TITLE");
        }
    }
        else
    {    if (keyWait > 10)
        {
                if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_RIGHT))
                {
                    m_yesNoSelect = 1 - m_yesNoSelect;
                    keyWait = 0;
                }
        }
        
        if (CheckHitKey(KEY_INPUT_RETURN))
            {
            if (m_yesNoSelect == 0)
            {

            }

                m_selectedItem = false;
                keyWait = 0;
            }
        }
    }



 

void ShopScene::Draw()
{
    if (m_backgroundImage != -1)
    {
        DrawGraph(0, 0, m_backgroundImage, FALSE);
    }
    else
    {
        DrawBox(0, 0, 1280, 720, GetColor(100, 50, 50), TRUE);
    }

    DrawString(560, 100, "ショップ", GetColor(25, 255, 255));

    const char* items[] = {
            "体力UP",
            "Bome増加",
          //  "攻撃速度UP",
           // "Bome範囲",
    };

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        int color = (i == m_selectedItem) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawString(550, 200 + i * 50, items[i], color);
    }

    DrawString(500, 600, "ENTER:決定 / ESC: タイトルへ", GetColor(255, 255, 255));
 
    if (m_selectedItem)
    {
        DrawBox(340, 250, 940, 420, GetColor(0, 0, 0), TRUE);
        DrawBox(340, 250, 940, 420, GetColor(255, 255, 255), FALSE);

        DrawString(520, 300, "このアイテムを購入しますか？", GetColor(255, 255, 255));
        int yesColor = (m_yesNoSelect == 0)? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        int noColor = (m_yesNoSelect == 1) ? GetColor(255, 255, 0): GetColor(200, 200, 200);
        DrawString(550, 360, "はい", yesColor);
        DrawString(650, 360, "いいえ", noColor);
    }
}

