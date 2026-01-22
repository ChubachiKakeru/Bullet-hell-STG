// BackGround.h
#pragma once
#include "../Library/GameObject.h"

class BackGround : public GameObject {
private:
    int hImage;
    int hImage2;
    float scrollY;
    float scrollSpeed;
    float offsetX;
    float offsetY;
    float scale;

public:
    BackGround();
    BackGround(float offX, float offY);
    ~BackGround();
    void Update() override;
    void Draw() override;
};