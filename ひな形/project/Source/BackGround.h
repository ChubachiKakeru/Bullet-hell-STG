#pragma once
#include "../Library/GameObject.h"

class BackGround : public GameObject
{
public:
    BackGround();
    ~BackGround();

    void Update() override;
    void Draw() override;

private:
    int hImage;
    int scrollY;
    float scrollSpeed;
};

