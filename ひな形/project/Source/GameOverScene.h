#pragma once
#include "../Library/SceneBase.h"

class GameOverScene : public SceneBase
{
public:
    GameOverScene();
    virtual ~GameOverScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    int m_gameOverImage;  // ゲームオーバー画像
    int m_timer;          // 演出用タイマー
};