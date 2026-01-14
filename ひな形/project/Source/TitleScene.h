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
};