#pragma once
#include"../Library/GameObject.h"

class Field;
class Bulett;

class Player : public GameObject
{
public:
    Player();
    Player(int sx, int sy);
    ~Player();

    void Initialize(float startX, float startY, Field* fieldPtr);
    void Update() override;
    void Draw() override;

    float GetX() const { return x; }
    float GetY() const { return y; }

    float GetRectW() const { return rectWidth; }
    float GetRectH() const { return rectHeight; }
    float GetRadius() const { return circleRadius; }

    // 判定サイズ設定
    void SetCollisionRect(float w, float h) { rectWidth = w; rectHeight = h; }
    void SetCollisionCircle(float r) { circleRadius = r; }

    // ダメージ処理
    void TakeDamage(int damage);
    int GetHP() const { return hp; }
    bool IsActive() const { return isActive; }

    void ShootBullet();

private:
    int hImage;
    float x, y;
    float velocity;
    bool onGround;
    Field* field;
    float shotTimer;     // 連射制限用

    // 判定サイズ
    float rectWidth;      // 矩形判定の幅
    float rectHeight;     // 矩形判定の高さ
    float circleRadius;   // 円形判定の半径

    // HP
    int hp;
    bool isActive;
};

