#pragma once
#include <cmath>

class CollisionManager {
public:
    // “ñ’iŠK”»’è: ‹éŒ`¨‰~Œ`
    static bool CheckCollision(
        float x1, float y1, float rectW1, float rectH1, float radius1,
        float x2, float y2, float rectW2, float rectH2, float radius2
    );

    // ‹éŒ`”»’è‚Ì‚İ
    static bool CheckRect(
        float x1, float y1, float w1, float h1,
        float x2, float y2, float w2, float h2
    );

    // ‰~Œ`”»’è‚Ì‚İ
    static bool CheckCircle(
        float x1, float y1, float r1,
        float x2, float y2, float r2
    );
};

