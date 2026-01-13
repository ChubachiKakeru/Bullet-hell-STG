#pragma once
#include "../Library/SceneBase.h"

class StageSelectScene : public SceneBase
{
public:
    StageSelectScene();
    virtual ~StageSelectScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    int m_backgroundImage;
    int m_selectedStage;
    static const int STAGE_COUNT = 5;
};