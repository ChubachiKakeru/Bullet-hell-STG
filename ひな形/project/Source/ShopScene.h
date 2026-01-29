#pragma once
#include "../Library/SceneBase.h"
#include "Player.h"
#include <DxLib.h>

class ShopScene : public SceneBase
{
public:
    ShopScene();
    virtual ~ShopScene();

    virtual void Update() override;
    virtual void Draw() override;
    bool m_isConfirm;     // 確認画面中
    int  m_yesNoSelect;  // 0: はい / 1: いいえ
    int m_selectItem;
    int  m_state;
    ShopScene(Player* p) : player(player) {}

    /* void irsHp() {
         player->AddHp(1);
     }

     void irsBomb() {
         player->AddBomb(1);
     }*/

private:
    int m_backgroundImage;
    int m_selectedItem;
    static const int ITEM_COUNT = 2;
    bool prevPush;

    Player* player;
    // サウンドパス
    const char* GAME_CANCEL_SOUND_PATH = "data/Sound/cancel.mp3";      //キャンセルジングル
    const char* GAME_CURSOR_SOUND_PATH = "data/Sound/cursor.mp3";      //選択ジングル
    const char* GAME_DECISION_SOUND_PATH = "data/Sound/decision.mp3";  //決定ジングル

    //  サウンドハンドル
    int CancelSoundHandle;      // キャンセルサウンドハンドル
    int CusorSoundHandle;       // 選択サウンドハンドル
    int DecisionSoundHandle;    // 決定サウンドハンドル
};