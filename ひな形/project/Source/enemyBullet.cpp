#include "enemyBullet.h"
#include "Player.h"
#include "Screen.h"
#include "Field.h"
#include <cmath>

// 通常のコンストラクタ
enemyBullet::enemyBullet(float sx, float sy, float vx, float vy, float bulletsize)
    : Bullet(sx, sy, vx, vy, bulletsize),
    currentImageType(0),  // デフォルトは通常弾
    isActive(true),
    isAlive(true),
    homingPower(0.0f),
    homingDuration(0),
    frameCount(0)
{
    hImage = LoadGraph("data/image/yaiba.png");
    hImageHoming = LoadGraph("data/image/yaiba3.png");  // 自機狙い用
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;
}

// 画像タイプ指定可能なコンストラクタ（追加）
enemyBullet::enemyBullet(float sx, float sy, float vx, float vy, float bulletsize, int imageType)
    : Bullet(sx, sy, vx, vy, bulletsize),
    currentImageType(imageType),  // 画像タイプを指定
    isActive(true),
    isAlive(true),
    frameCount(0)
{
    hImage = LoadGraph("data/image/yaiba.png");
    hImageHoming = LoadGraph("data/image/yaiba3.png");  // 自機狙い用
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;

    // ホーミング弾の設定（弱めに変更）
    if (currentImageType == 1) {
        homingPower = 0.08f;      // 0.2f から 0.08f に変更（約半分以下）
        homingDuration = 150;     // 240 から 150 に変更（2.5秒間追尾）
    }
    else {
        homingPower = 0.0f;
        homingDuration = 0;
    }
}

// デストラクタ
enemyBullet::~enemyBullet()
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

void enemyBullet::Update() {
    // 基底クラスの更新を呼ぶ
    Bullet::Update();
    if (!isActive) return;

    // ホーミング処理
    if (currentImageType == 1 && frameCount < homingDuration) {
        Player* player = FindGameObject<Player>();
        if (player != nullptr) {
            // プレイヤーへの方向ベクトルを計算
            float targetX = player->GetX();
            float targetY = player->GetY();

            float dx = targetX - (x + 40);
            float dy = targetY - (y + 40);
            float length = sqrt(dx * dx + dy * dy);

            if (length > 0) {
                // 正規化
                dx /= length;
                dy /= length;

                // 現在の速度を徐々にプレイヤー方向に曲げる
                velocityX += dx * homingPower;
                velocityY += dy * homingPower;

                // 速度の大きさを一定に保つ
                float currentSpeed = sqrt(velocityX * velocityX + velocityY * velocityY);
                float targetSpeed = 5.5f;

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
    if (p != nullptr && p->IsHit(x + 40, y + 40, 5))
    {
        DestroyMe();
        return;
    }

    // ステージ範囲外判定（追尾弾は範囲を広げる、または追尾中は判定しない）
    if (currentImageType == 1) {
        // 追尾弾：追尾期間中は範囲外判定をしない、追尾終了後は通常判定
        if (frameCount >= homingDuration) {
            // 追尾終了後は大きめの範囲で判定
            if (x < Field::STAGE_LEFT - 200 || x > Field::STAGE_RIGHT + 200 ||
                y < -200 || y > Field::STAGE_BOTTOM + 200) {
                DestroyMe();
                return;
            }
        }
        // 追尾中は範囲外判定をスキップ（画面外に出ても追尾し続ける）
    }
    else {
        // 通常弾：従来通りの判定
        if (x < Field::STAGE_LEFT - 50 || x > Field::STAGE_RIGHT + 50 ||
            y < -1 || y > Field::STAGE_BOTTOM + 100) {
            DestroyMe();
            return;
        }
    }
}
void enemyBullet::Draw() {
    // 画像タイプに応じて描画する画像を選択
    int imageToUse = (currentImageType == 1) ? hImageHoming : hImage;

    // 敵の弾の描画
    if (isActive && imageToUse != -1) {
        DrawGraph((int)x, (int)y, imageToUse, TRUE);
    }
    else if (isActive) {
        // 画像がロードできなかった場合の代替描画
        int color = (currentImageType == 1) ? GetColor(255, 0, 0) : GetColor(255, 255, 0);
        DrawCircle((int)(x + 40), (int)(y + 40), (int)size, color, TRUE);
    }

    // デバッグ用の当たり判定表示
    DrawCircle((int)(x + 41), (int)(y + 40), 40, GetColor(255, 0, 255), FALSE);

    // ホーミング弾には追加の視覚効果（オプション）
    if (isActive && currentImageType == 1 && frameCount < homingDuration) {
        // ホーミング中は赤い外枠を表示
        DrawCircle((int)(x + 41), (int)(y + 40), 45, GetColor(255, 0, 0), FALSE);
    }
}