#include "Collision.h"

bool Collision::MyCircleCheck(float x1, float y1, float r1, float x2, float y2, float r2)
{
    // ’†SŠÔ‚Ì‹——£‚Ì2æ‚ğŒvZ
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distanceSquared = dx * dx + dy * dy;

    // ”¼Œa‚Ì˜a‚Ì2æ‚ğŒvZ
    float radiusSum = r1 + r2;
    float radiusSumSquared = radiusSum * radiusSum;

    // “–‚½‚è”»’è
    return distanceSquared < radiusSumSquared;
}
