#include "ShopScene.h"
#include "Player.h"
#include"Bomb.h"
#include "../Library/SceneManager.h"
#include "StageSelectScene.h"
#include "Common.h"


enum ShopState
{
    //    SHOP_SELECT,
    SHOP_CONFIRM
};

ShopScene::ShopScene()
    : m_backgroundImage(-1), m_selectedItem(0)
    , m_isConfirm(false), m_yesNoSelect(0),
    m_selectItem(0),   // 追加
    m_state(0),        // 追加
    prevPush(false),   // 追加
    player(0)
{
    m_backgroundImage = LoadGraph("data/image/shop.jpg");

    CancelSoundHandle = LoadSoundMem(GAME_CANCEL_SOUND_PATH);
    CusorSoundHandle = LoadSoundMem(GAME_CURSOR_SOUND_PATH);
    DecisionSoundHandle = LoadSoundMem(GAME_DECISION_SOUND_PATH);
}

ShopScene::~ShopScene()
{
    if (m_backgroundImage != -1)
    {
        DeleteGraph(m_backgroundImage);
    }
    DeleteSoundMem(CancelSoundHandle);
    DeleteSoundMem(CusorSoundHandle);
    //DeleteSoundMem(DecisionSoundHandle);
}

void ShopScene::Update()
{
    static int keyWait = 0;
    keyWait++;

    bool enter = CheckHitKey(KEY_INPUT_RETURN);

    // ★ Player を毎フレーム取得
    player = FindGameObject<Player>();

    // =========================
    // 通常状態
    // =========================
    if (!m_isConfirm)
    {
        if (keyWait > 10)
        {
            if (CheckHitKey(KEY_INPUT_UP))
            {
                m_selectedItem = (m_selectedItem - 1 + ITEM_COUNT) % ITEM_COUNT;
                keyWait = 0;
                PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
            }
            if (CheckHitKey(KEY_INPUT_DOWN))
            {
                m_selectedItem = (m_selectedItem + 1) % ITEM_COUNT;
                keyWait = 0;
                PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
            }
        }

        if (enter && !prevPush)
        {
            m_isConfirm = true;
            m_yesNoSelect = 0;
            keyWait = 0;
        }

        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            SceneManager::ChangeScene("PLAY");
            PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);
        }
    }
    // =========================
    // 確認ウィンドウ
    // =========================
    else
    {
        if (keyWait > 10)
        {
            if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_RIGHT))
            {
                m_yesNoSelect = 1 - m_yesNoSelect;
                keyWait = 0;
                PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
            }
        }

        if (enter && !prevPush)
        {
            if (m_yesNoSelect == 0) // はい
            {
                PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);
                Common* common = FindGameObject<Common>();
                if (common)
                {
                    switch (m_selectedItem)
                    {
                    case 0: // HP +1
                        Player::UpgradeMaxHP(1); // 永続強化
                        common->carryHp += 1;    // ★回復予約
                        break;

                    case 1: // Bomb +1
                        Player::UpgradeInitialBombCount(1); // 永続強化
                        common->carryBomb += 1;              // ★回復予約
                        break;
                    }
                }
            }


            m_isConfirm = false;
            keyWait = 0;
        }
    }

    prevPush = enter;
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
    SetFontSize(50);
    DrawString(535, 180, "ショップ", GetColor(25, 255, 255));
    SetFontSize(24);

    const char* items[] = {
        "体力+1",
        "Bome+1",
        //  "攻撃速度UP",
        // "Bome範囲",
    };

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        SetFontSize(60);
        int color = (i == m_selectedItem) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawString(530, 500 + i * 100, items[i], color);
    }
    SetFontSize(40);
    DrawString(200, 1000, "↑↓キーで選択/ ENTER:決定 / ESC: ステージ2へ", GetColor(255, 255, 255));

    if (m_isConfirm)
    {
        DrawBox(340, 270, 940, 500, GetColor(0, 0, 0), TRUE);
        DrawBox(340, 270, 940, 500, GetColor(255, 255, 255), FALSE);
        DrawString(355, 350, "このアイテムを購入しますか？", GetColor(255, 255, 255));

        int yesColor = (m_yesNoSelect == 0) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        int noColor = (m_yesNoSelect == 1) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

        DrawString(470, 430, "はい", yesColor);
        DrawString(700, 430, "いいえ", noColor);
    }
}