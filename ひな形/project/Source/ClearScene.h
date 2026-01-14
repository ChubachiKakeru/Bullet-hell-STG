#pragma once
#include "../Library/SceneBase.h"

class ClearScene : public SceneBase
{
public:
    ClearScene();
    virtual ~ClearScene();

    virtual void Update() override;
    virtual void Draw() override;

private:
    int m_clearImage;
    int m_timer;
};