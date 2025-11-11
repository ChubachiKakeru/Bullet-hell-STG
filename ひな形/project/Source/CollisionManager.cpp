#include "CollisionManager.h"

bool CollisionManager::CheckCollision(
    float x1, float y1, float rectW1, float rectH1, float radius1,
    float x2, float y2, float rectW2, float rectH2, float radius2
) {
    // 第1段階: 矩形判定（大まかな判定）
    if (!CheckRect(x1, y1, rectW1, rectH1, x2, y2, rectW2, rectH2)) {
        return false;  // 矩形で当たってないので終了
    }

    // 第2段階: 円形判定（詳細な判定）
    return CheckCircle(x1, y1, radius1, x2, y2, radius2);
}

bool CollisionManager::CheckRect(
    float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2
) {
    return (x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        y1 + h1 > y2);
}

bool CollisionManager::CheckCircle(
    float x1, float y1, float r1,
    float x2, float y2, float r2
) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

