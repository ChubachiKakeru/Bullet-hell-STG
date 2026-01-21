#include "BackGround.h"
#include "DxLib.h"

BackGround::BackGround() : GameObject() {
    hImage = LoadGraph("data/image/bg.png");
    hImage2 = LoadGraph("data/image/bg.png");
    scrollY = 0.0f;
    scrollSpeed = 1.0f;   //ゆっくり（0.5ピクセル/フレーム）
    offsetX = 0.0f;     // 中央
    offsetY = 0.0f;     // 上端
    scale = 1.0f;       // 等倍
}

BackGround::BackGround(float offX, float offY) : GameObject() {
    hImage = LoadGraph("data/image/bg.png");
    hImage2 = LoadGraph("data/image/bg.png");
    scrollY = 0.0f;
    scrollSpeed = 0.5f;
    offsetX = offX;
    offsetY = offY;
    scale = 1.0f;
}

BackGround::~BackGround() {
    if (hImage != -1) {
        DeleteGraph(hImage);
    }
    if (hImage2 != -1 && hImage2 != hImage) {
        DeleteGraph(hImage2);
    }
}

void BackGround::Update() {
    // スクロール更新
    scrollY += scrollSpeed;
    // 背景画像の高さを超えたらリセット
    int bgHeight = 960;
    if (scrollY >= bgHeight) {
        scrollY -= bgHeight;
    }
}

void BackGround::Draw() {
    if (hImage == -1) return;
    int bgHeight = 960;       // 表示する高さ
    int screenWidth = 840;    // 表示する幅
    int screenHeight = 1280;  // 画面高さ

    // スクロール位置を計算
    int drawY1 = (int)offsetY - (int)scrollY;

    // 画面全体をカバーするために複数枚描画
    // 上から下まで確実にカバー
    for (int i = -1; i <= 2; i++) {
        int drawY = drawY1 + (i * bgHeight);

        DrawExtendGraph(
            (int)offsetX,
            drawY,
            (int)offsetX + screenWidth,
            drawY + bgHeight,
            hImage,
            TRUE
        );
    }
}