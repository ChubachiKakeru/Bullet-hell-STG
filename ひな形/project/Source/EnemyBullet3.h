#pragma once
#include "Bullet.h"

class EnemyBullet3 : public Bullet
{
public:
    // 通常コンストラクタ
    EnemyBullet3(float sx, float sy, float vx, float vy, float bulletsize);

    // 反射機能付きコンストラクタ
    EnemyBullet3(float sx, float sy, float vx, float vy, float bulletsize, bool enableReflect);

    ~EnemyBullet3();

    void Update() override;
    void Draw() override;

private:
    bool canReflect;
    int reflectCount;
};