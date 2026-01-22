#include "EnemyBullet2.h"
#include "Player.h"
#include "Screen.h"
#include "Field.h"
#include <cmath>

// 通常のコンストラクタ
EnemyBullet2::EnemyBullet2(float sx, float sy, float vx, float vy, float bulletsize)
    : Bullet(sx, sy, vx, vy, bulletsize),
    currentImageType(0),
    isActive(true),
    isAlive(true),
    homingPower(0.0f),
    homingDuration(0),
    frameCount(0)
{
    hImage = LoadGraph("data/image/fire.png");
    hImageHoming = LoadGraph("data/image/flame.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;
}

// 画像タイプ指定可能なコンストラクタ
EnemyBullet2::EnemyBullet2(float sx, float sy, float vx, float vy, float bulletsize, int imageType)
    : Bullet(sx, sy, vx, vy, bulletsize),
    currentImageType(imageType),
    isActive(true),
    isAlive(true),
    frameCount(0)
{
    hImage = LoadGraph("data/image/fire.png");
    hImageHoming = LoadGraph("data/image/flame.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;

    // ホーミング弾の設定
    if (currentImageType == 1) {
        homingPower = 0.08f;
        homingDuration = 150;
    }
    else {
        homingPower = 0.0f;
        homingDuration = 0;
    }
}

// デストラクタ
EnemyBullet2::~EnemyBullet2()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
        hImage = -1;
    }
    if (hImageHoming != -1) {
        DeleteGraph(hImageHoming);
        hImageHoming = -1;
    }
}

void EnemyBullet2::Update() {
    Bullet::Update();
    if (!isActive) return;

    // 弾の中心位置を計算（画像タイプによって異なる）
    float bulletCenterX, bulletCenterY;
    if (currentImageType == 1) {
        // 自機狙い弾：150x150
        bulletCenterX = x + 75.0f;
        bulletCenterY = y + 75.0f;
    }
    else {
        // 通常弾：76x83
        bulletCenterX = x + 38.0f;
        bulletCenterY = y + 41.5f;
    }

    // ホーミング処理
    if (currentImageType == 1 && frameCount < homingDuration) {
        Player* player = FindGameObject<Player>();
        if (player != nullptr) {
            // プレイヤーの中心位置を取得（150x150画像の中心）
            float targetX = player->GetX() + 75.0f;
            float targetY = player->GetY() + 75.0f;

            // プレイヤーへの方向ベクトルを計算
            float dx = targetX - bulletCenterX;
            float dy = targetY - bulletCenterY;
            float length = sqrt(dx * dx + dy * dy);

            if (length > 0) {
                // 正規化
                dx /= length;
                dy /= length;

                // 現在の速度を徐々にプレイヤー方向に曲げる
                velocityX += dx * homingPower;
                velocityY += dy * homingPower;

                // 速度の大きさを一定に保つ（速度を上げる）
                float currentSpeed = sqrt(velocityX * velocityX + velocityY * velocityY);
                float targetSpeed = 8.0f;  // 5.5fから8.0fに変更

                if (currentSpeed > 0.1f) {
                    velocityX = (velocityX / currentSpeed) * targetSpeed;
                    velocityY = (velocityY / currentSpeed) * targetSpeed;
                }
            }
        }
    }

    frameCount++;

    // 弾の移動
    x += velocityX;
    y += velocityY;

    // プレイヤーとの当たり判定
    Player* p = FindGameObject<Player>();
    if (p != nullptr) {
        // 弾の中心位置（画像タイプによって異なる）
        float bulletCenterX, bulletCenterY;
        if (currentImageType == 1) {
            // 自機狙い弾：150x150
            bulletCenterX = x + 75.0f;
            bulletCenterY = y + 75.0f;
        }
        else {
            // 通常弾：76x83
            bulletCenterX = x + 38.0f;
            bulletCenterY = y + 41.5f;
        }

        // プレイヤーの中心位置（150x150画像の中心）
        float playerCenterX = p->GetX() + 75.0f;
        float playerCenterY = p->GetY() + 75.0f;

        // 距離による判定
        float dx = bulletCenterX - playerCenterX;
        float dy = bulletCenterY - playerCenterY;
        float distance = sqrt(dx * dx + dy * dy);

        // 当たり判定の半径（弾のタイプに応じて調整）
        float hitRadius;
        if (currentImageType == 1) {
            hitRadius = 50.0f;  // 自機狙い弾
        }
        else {
            hitRadius = 40.0f;  // 通常弾
        }

        if (distance < hitRadius) {
            p->TakeDamage(1);
            DestroyMe();
            return;
        }
    }

    // ステージ範囲外判定
    if (currentImageType == 1) {
        // 追尾弾：かなり広い範囲まで許容（画面外でも追尾し続ける）
        if (x < Field::STAGE_LEFT - 500 || x > Field::STAGE_RIGHT + 500 ||
            y < -500 || y > Field::STAGE_BOTTOM + 1000) {
            DestroyMe();
            return;
        }
    }
    else {
        // 通常弾：76x83サイズに合わせた判定
        if (x < Field::STAGE_LEFT - 100 || x > Field::STAGE_RIGHT + 70 ||
            y < -100 || y > Field::STAGE_BOTTOM + 300) {
            DestroyMe();
            return;
        }
    }
}

void EnemyBullet2::Draw() {
    // 画像タイプに応じて描画する画像を選択
    int imageToUse = (currentImageType == 1) ? hImageHoming : hImage;

    // 敵の弾の描画
    if (isActive && imageToUse != -1) {
        DrawGraph((int)x, (int)y, imageToUse, TRUE);
    }
    else if (isActive) {
        // 画像がロードできなかった場合の代替描画
        int color = (currentImageType == 1) ? GetColor(255, 0, 0) : GetColor(255, 255, 0);
        if (currentImageType == 1) {
            DrawCircle((int)(x + 75.0f), (int)(y + 75.0f), 75, color, TRUE);
        }
        else {
            DrawCircle((int)(x + 38.0f), (int)(y + 41.5f), 38, color, TRUE);
        }
    }

    // デバッグ用の当たり判定表示
    if (currentImageType == 1) {
        DrawCircle((int)(x + 75.0f), (int)(y + 75.0f), 50, GetColor(255, 0, 255), FALSE);
    }
    else {
        DrawCircle((int)(x + 38.0f), (int)(y + 41.5f), 40, GetColor(255, 0, 255), FALSE);
    }

    // ホーミング弾には追加の視覚効果
    if (isActive && currentImageType == 1 && frameCount < homingDuration) {
        DrawCircle((int)(x + 75.0f), (int)(y + 75.0f), 60, GetColor(255, 0, 0), FALSE);
    }
}