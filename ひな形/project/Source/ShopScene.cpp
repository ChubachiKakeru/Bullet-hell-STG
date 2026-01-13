#include "ShopScene.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

ShopScene::ShopScene()
    : m_backgroundImage(-1)
    , m_selectedItem(0)
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

    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        SceneManager::ChangeScene("TITLE");
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

    DrawString(550, 100, "ショップ", GetColor(255, 255, 255));

    const char* items[] = {
        "体力回復薬 - 100G",
        "攻撃力UP - 200G",
        "防御力UP - 200G",
        "スピードUP - 150G",
        "特殊武器 - 500G",
        "復活アイテム - 300G"
    };

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        int color = (i == m_selectedItem) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawString(450, 200 + i * 50, items[i], color);
    }

    DrawString(450, 600, "SPACE: 購入 / ESC: タイトルへ", GetColor(255, 255, 255));
}