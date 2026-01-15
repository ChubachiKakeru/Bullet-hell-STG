#pragma once
#include "../Library/SceneBase.h"

class ShopScene : public SceneBase
{
public:
    ShopScene();
    virtual ~ShopScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    int m_backgroundImage;
    int m_selectedItem;
    static const int ITEM_COUNT = 4;
};