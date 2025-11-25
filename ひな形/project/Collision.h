#pragma once
#include"../project/Library/GameObject.h"

class Collision : public GameObject
{
public:
    // ‰~Œ`“–‚½‚è”»’è
    static bool MyCircleCheck(float x1, float y1, float r1,
        float x2, float y2, float r2);
};

