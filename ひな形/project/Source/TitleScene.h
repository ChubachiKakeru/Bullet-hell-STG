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
    int m_blinkTimer;           // ★点滅用タイマー★
    bool m_startPressed;        // ★Pキー押下フラグ★
    int m_fadeOutTimer;         // ★フェードアウト用タイマー★

    // ★隠しコマンド用変数（追加）★
    static const int SECRET_COMMAND_LENGTH = 6;  // コマンドの長さ
    int m_secretCommandIndex;                    // 現在の入力位置
    int m_secretKeyWait;                         // キー入力待機時間
    bool m_secretUnlocked;                       // 隠しステージ解放フラグ

    // サウンドパス
    const char* GAME_TITLE_SOUND_PATH = "data/Sound/title.mp3";      //タイトルジングル

    // サウンドハンドル
    int titleSoundHandle;		//	タイトルサウンドハンドル

    // ★隠しコマンドのチェック関数（追加）★
    void CheckSecretCommand();
};