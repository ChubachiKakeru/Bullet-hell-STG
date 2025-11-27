#include "playerBullet.h"
#include"Boss1.h"

playerBullet::playerBullet(float sx, float sy, float vx, float vy, float bulletsize) : Bullet( sx, sy, vx,  vy,  bulletsize)
{
    hImage = LoadGraph("data/image/siratama.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;

}
void playerBullet::Update() {
    // 基底クラスの更新を呼ぶ
    Bullet::Update();
    if (!isActive) return;

    // 弾の移動
    x += velocityX;
    y += velocityY;
    Boss1* b = FindGameObject<Boss1>();
    if (b->IsHit(x + 40, y + 40, 10))
    {
        DestroyMe();
    }
    
    

    // プレイヤー弾特有の処理があればここに追加
    // 例：画面外判定
    if (y < 0) {
        DestroyMe();
    }
}

void playerBullet::Draw() {
    // プレイヤーの弾の描画（青色など）
    // DrawCircle(x, y, size, BLUE);
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle(x + 40, y + 40, 30, GetColor(0, 255, 255), FALSE);
    
}