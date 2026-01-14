#include "DebugSceneSelector.h"
#include "../Library/SceneManager.h"
#include <DxLib.h>

bool DebugSceneSelector::m_isActive = false;
int DebugSceneSelector::m_selectedScene = 0;

void DebugSceneSelector::Update()
{
    // F1キーでデバッグメニューの表示切り替え
    static bool prevF1 = false;
    bool currentF1 = (CheckHitKey(KEY_INPUT_F1) != 0);
    if (currentF1 && !prevF1)
    {
        m_isActive = !m_isActive;
    }
    prevF1 = currentF1;

    if (!m_isActive) return;

    // 数字キーでシーン遷移
    if (CheckHitKey(KEY_INPUT_1))
    {
        SceneManager::ChangeScene("TITLE");
        m_isActive = false;
    }
    if (CheckHitKey(KEY_INPUT_2))
    {
        SceneManager::ChangeScene("STAGESELECT");
        m_isActive = false;
    }
    if (CheckHitKey(KEY_INPUT_3))
    {
        SceneManager::ChangeScene("SHOP");
        m_isActive = false;
    }
    if (CheckHitKey(KEY_INPUT_4))
    {
        SceneManager::ChangeScene("PLAY");
        m_isActive = false;
    }
    if (CheckHitKey(KEY_INPUT_5))
    {
        SceneManager::ChangeScene("CLEAR");
        m_isActive = false;
    }
    if (CheckHitKey(KEY_INPUT_6))
    {
        SceneManager::ChangeScene("GAMEOVER");
        m_isActive = false;
    }
}

void DebugSceneSelector::Draw()
{
    if (!m_isActive) return;

    // 半透明の背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(300, 150, 980, 570, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 枠線
    DrawBox(300, 150, 980, 570, GetColor(255, 255, 0), FALSE);

    // タイトル
    DrawString(500, 170, "=== デバッグシーン選択 ===", GetColor(255, 255, 0));
    DrawString(420, 200, "F1: メニューを閉じる", GetColor(200, 200, 200));

    // シーン一覧
    DrawString(400, 250, "1: タイトルシーン", GetColor(255, 255, 255));
    DrawString(400, 290, "2: ステージ選択シーン", GetColor(255, 255, 255));
    DrawString(400, 330, "3: ショップシーン", GetColor(255, 255, 255));
    DrawString(400, 370, "4: プレイシーン", GetColor(255, 255, 255));
    DrawString(400, 410, "5: クリアシーン", GetColor(255, 255, 255));
    DrawString(400, 450, "6: ゲームオーバーシーン", GetColor(255, 255, 255));

    DrawString(380, 510, "数字キーを押してシーンに遷移", GetColor(200, 200, 200));
}