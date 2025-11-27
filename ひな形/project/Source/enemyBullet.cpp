#include "enemyBullet.h"
#include"Player.h"
#include"Screen.h"

enemyBullet::enemyBullet(float sx, float sy, float vx, float vy, float bulletsize): Bullet(sx, sy, vx, vy, bulletsize)
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
    Player* p = FindGameObject<Player>();
    if (p->IsHit(x + 40, y + 40, 5))
    {
        DestroyMe();
    }

    // 敵弾特有の処理があればここに追加
    // 例：画面外判定
    if (y > Screen::HEIGHT) {  // 画面下端
        DestroyMe();
    }
}

void enemyBullet::Draw() {
    // 敵の弾の描画（赤色など）
    // DrawCircle(x, y, size, RED);
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle(x + 41, y + 40, 40, GetColor(255, 0, 255), FALSE);

}