#include "enemyBullet.h"
#include "Player.h"
#include "Screen.h"
#include "Field.h"

enemyBullet::enemyBullet(float sx, float sy, float vx, float vy, float bulletsize) : Bullet(sx, sy, vx, vy, bulletsize)
{
    hImage = LoadGraph("data/image/yaiba.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;
}

void enemyBullet::Update() {
    // 基底クラスの更新を呼ぶ
    Bullet::Update();
    if (!isActive) return;

    // 弾の移動
    x += velocityX;
    y += velocityY;

    // プレイヤーとの当たり判定
    Player* p = FindGameObject<Player>();
    if (p != nullptr && p->IsHit(x + 40, y + 40, 5))
    {
        DestroyMe();
        return;
    }

    // ステージ範囲外判定（余裕を持たせて50px外まで）
    if (x < Field::STAGE_LEFT - 50 || x > Field::STAGE_RIGHT + 50 ||
        y < Field::STAGE_TOP - 50 || y > Field::STAGE_BOTTOM -50) {
        DestroyMe();
        return;
    }
}

void enemyBullet::Draw() {
    // 敵の弾の描画
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle(x + 41, y + 40, 40, GetColor(255, 0, 255), FALSE);
}