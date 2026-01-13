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
    int m_titleImage;  // ƒ^ƒCƒgƒ‹‰æ‘œ
	int hImage;
	int orgW, orgH;
	int newW, newH;
	int font;
};
