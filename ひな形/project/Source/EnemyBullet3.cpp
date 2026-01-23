#include "EnemyBullet3.h"
#include "Player.h"
#include "Screen.h"
#include "Field.h"
#include "../Library/GameObject.h"
#include <cmath>

// 通常コンストラクタ（反射なし）
EnemyBullet3::EnemyBullet3(float sx, float sy, float vx, float vy, float bulletsize)
    : Bullet(sx, sy, vx, vy, bulletsize)
{
    hImage = LoadGraph("data/image/fire.png");
    x = sx;
    y = sy;
    size = bulletsize;
    isActive = true;
    isAlive = true;
    canReflect = false;
    reflectCount = 0;
    velocityX = vx;
    velocityY = vy;
}

// 反射機能付きコンストラクタ
EnemyBullet3::EnemyBullet3(float sx, float sy, float vx, float vy, float bulletsize, bool enableReflect)
    : Bullet(sx, sy, vx, vy, bulletsize)
{
    hImage = LoadGraph("data/image/fire.png");
    x = sx;
    y = sy;
    size = bulletsize;
    isActive = true;
    isAlive = true;
    canReflect = enableReflect;
    reflectCount = 0;
    velocityX = vx;
    velocityY = vy;
}

EnemyBullet3::~EnemyBullet3()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
        hImage = -1;
    }
}

void EnemyBullet3::Update()
{
    if (!isActive) return;

    // 移動
    x += velocityX;
    y += velocityY;

    // 反射処理
    if (canReflect && reflectCount < 3) {
        bool reflected = false;

        // 左右の壁で反射
        if (x < Field::STAGE_LEFT || x > Field::STAGE_RIGHT) {
            velocityX = -velocityX;
            reflectCount++;
            reflected = true;

            // 壁の中に埋まらないように補正
            if (x < Field::STAGE_LEFT) {
                x = Field::STAGE_LEFT;
            }
            if (x > Field::STAGE_RIGHT) {
                x = Field::STAGE_RIGHT;
            }
        }

        // 上下の壁で反射
        if (y < Field::STAGE_TOP || y > Field::STAGE_BOTTOM) {
            velocityY = -velocityY;
            if (!reflected) {  // 同時に2回カウントしないように
                reflectCount++;
            }

            // 壁の中に埋まらないように補正
            if (y < Field::STAGE_TOP) {
                y = Field::STAGE_TOP;
            }
            if (y > Field::STAGE_BOTTOM) {
                y = Field::STAGE_BOTTOM;
            }
        }
    }

    // プレイヤーとの当たり判定
    Player* player = FindGameObject<Player>();
    if (player != nullptr) {
        // 弾の中心位置
        float bulletCenterX = x + size / 2.0f;
        float bulletCenterY = y + size / 2.0f;

        // プレイヤーの中心位置（150x150画像の中心と仮定）
        float playerCenterX = player->GetX() + 75.0f;
        float playerCenterY = player->GetY() + 75.0f;

        // 距離による判定
        float dx = bulletCenterX - playerCenterX;
        float dy = bulletCenterY - playerCenterY;
        float distance = sqrt(dx * dx + dy * dy);

        // 当たり判定の半径（弾のサイズに応じて調整）
        float hitRadius = size / 2.0f + 30.0f;

        if (distance < hitRadius) {
            player->TakeDamage(1);
            isActive = false;
            DestroyMe();
            return;
        }
    }

    // 画面外判定（EnemyBullet2と同様）
    // 反射弾の場合：かなり広い範囲まで許容
    if (canReflect) {
        if (x < Field::STAGE_LEFT - 500 || x > Field::STAGE_RIGHT + 500 ||
            y < -500 || y > Field::STAGE_BOTTOM + 1000) {
            isActive = false;
            DestroyMe();
        }
    }
    else {
        // 通常弾：標準的な範囲
        if (x < Field::STAGE_LEFT - 100 || x > Field::STAGE_RIGHT + 100 ||
            y < -100 || y > Field::STAGE_BOTTOM + 300) {
            isActive = false;
            DestroyMe();
        }
    }
}

void EnemyBullet3::Draw()
{
    if (isActive && hImage != -1) {
        // サイズをさらに大きく表示（size / 10.0 で約6倍）
        DrawRotaGraph((int)x, (int)y, size / 10.0, 0.0, hImage, TRUE);
    }
}