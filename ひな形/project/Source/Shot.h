#pragma once
#include <iostream>
#include"../Library/GameObject.h"

class Shot {
private:
    float x, y;
    bool isActive;

public:
    Shot(float startX, float startY);
    void Update();
    void Draw();
    bool IsActive() const { return isActive; }
};


