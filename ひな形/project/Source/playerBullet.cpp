#include "playerBullet.h"
#include "Boss1.h"
#include "zako1.h"
#include "EnemyManager.h"

playerBullet::playerBullet(float sx, float sy, float vx, float vy, float bulletsize)
    : Bullet(sx, sy, vx, vy, bulletsize)
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

    // EnemyManagerから全ての敵を取得して当たり判定
    EnemyManager* em = FindGameObject<EnemyManager>();
    if (em) {
        const auto& enemies = em->GetEnemies();
        for (Enemy* enemy : enemies) {
            if (enemy && !enemy->IsDead()) {
                // 敵の種類に応じてキャスト
                Boss1* boss = dynamic_cast<Boss1*>(enemy);
                if (boss && boss->IsHit(x + 40, y + 40, 10)) {
                    DestroyMe();
                    return;
                }

                // zako1の当たり判定
                zako1* zako = dynamic_cast<zako1*>(enemy);
                if (zako && zako->IsHit(x + 40, y + 40, 10)) {
                    DestroyMe();
                    return;
                }
            }
        }
    }

    // 画面外判定
    if (y < 0) {
        DestroyMe();
    }
}

void playerBullet::Draw() {
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle((int)(x + 40), (int)(y + 40), 30, GetColor(0, 255, 255), FALSE);
}