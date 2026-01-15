#pragma once
#include "../Library/SceneBase.h"

class ShopScene : public SceneBase
{
public:
    ShopScene();
    virtual ~ShopScene();

    virtual void Update() override;
    virtual void Draw() override;
    bool m_isConfirm;     // Šm”F‰æ–Ê’†
    int  m_yesNoSelect;  // 0: ‚Í‚¢ / 1: ‚¢‚¢‚¦

private:
    int m_backgroundImage;
    int m_selectedItem;
    static const int ITEM_COUNT = 2;
};