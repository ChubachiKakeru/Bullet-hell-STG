#pragma once
#include "../Library/SceneBase.h"

class TitleScene : public SceneBase
{
public:
    TitleScene();
    virtual ~TitleScene();
    virtual void Update() override;
    virtual void Draw() override;

private:
    int m_titleImage;           // タイトル画像のハンドル
    int hImage;
    int orgW, orgH;
    int newW, newH;
    int font;
    bool m_showDebugMenu;       // デバッグメニュー表示フラグ
    int m_debugKeyWait;         // デバッグメニュー切り替えキーの待機時間
    int m_blinkTimer;           // ★追加: 点滅用タイマー★
    bool m_startPressed;        // ★追加: Pキー押下フラグ★
    int m_fadeOutTimer;         // ★追加: フェードアウト用タイマー★
    // サウンドパス
    const char* GAME_TITLE_SOUND_PATH = "data/Sound/title.mp3";      //タイトルジングル
    //  サウンドハンドル
    int titleSoundHandle;		//	1面サウンドハンドル
};