#pragma once
#include "Bullet.h"

class enemyBullet : public Bullet
{
public:
    // 通常のコンストラクタ（デフォルトは通常弾）
    enemyBullet(float sx, float sy, float vx, float vy, float bulletsize = 8.0f);

    // 画像タイプ指定可能なコンストラクタ（追加）
    enemyBullet(float sx, float sy, float vx, float vy, float bulletsize, int imageType);

    // デストラクタ
    ~enemyBullet();

    // オーバーライド
    void Update() override;
    void Draw() override;

private:
    float x, y;
    int hImage;           // 通常弾の画像
    int hImageHoming;     // 自機狙い弾の画像
    int currentImageType; // 0=通常, 1=ホーミング
    float velocityX, velocityY;
    float size;
    bool isActive;
    bool isAlive;

    // ホーミング用の追加メンバ
    float homingPower;    // 追尾力
    int homingDuration;   // 追尾する時間（フレーム数）
    int frameCount;       // 経過フレーム数
};