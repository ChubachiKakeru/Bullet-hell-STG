#pragma once
#include "../Library/GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(int sx, int sy, float vx, float vy);
    ~Bullet();

    void Update() override;
    void Draw() override;

    bool IsActive() const { return isActive; }
    float GetX() const { return x; }
    float GetY() const { return y; }

    // 判定サイズ取得
    float GetRectW() const { return rectWidth; }
    float GetRectH() const { return rectHeight; }
    float GetRadius() const { return circleRadius; }

    // 判定サイズ設定
    void SetCollisionRect(float w, float h) { rectWidth = w; rectHeight = h; }
    void SetCollisionCircle(float r) { circleRadius = r; }

    void Deactivate() { isActive = false; }

private:
    int hImage;
    float x, y;
    float velocityX, velocityY;
    bool isActive;
    // 判定サイズ
    float rectWidth;
    float rectHeight;
    float circleRadius;
};

