#include "BackGround.h"
#include "DxLib.h"

BackGround::BackGround() : GameObject() {
    hImage = LoadGraph("data/image/hosizora.png");
    hImage2 = LoadGraph("data/image/hosizora2.png");
    scrollY = 0.0f;
    scrollSpeed = 1.0f;   //ゆっくり（0.5ピクセル/フレーム）
    offsetX = 0.0f;     // 中央
    offsetY = 0.0f;     // 上端
    scale = 1.0f;       // 等倍
}

BackGround::BackGround(float offX, float offY) : GameObject() {
    hImage = LoadGraph("data/image/hosizora.png");
    hImage2 = LoadGraph("data/image/hosizora2.png");
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

    int bgHeight = 960;
    int screenWidth = 840;
    int screenHeight = 960;

    // スクロール位置を計算
    int drawY1 = (int)offsetY - (int)scrollY;
    int drawY2 = drawY1 + bgHeight;

    // 1枚目の背景
    DrawExtendGraph(
        (int)offsetX,
        drawY1,
        (int)offsetX + screenWidth,
        drawY1 + bgHeight,
        hImage,
        TRUE
    );

    // 2枚目の背景（シームレスループ用）
    // 1枚目が画面外に出たら2枚目を表示
    if (drawY1 + bgHeight < screenHeight) {
        DrawExtendGraph(
            (int)offsetX,
            drawY2,
            (int)offsetX + screenWidth,
            drawY2 + bgHeight,
            hImage,  // 同じ画像を使う
            TRUE
        );
    }

    // 上にスクロールする場合も考慮（念のため）
    if (drawY1 > 0) {
        DrawExtendGraph(
            (int)offsetX,
            drawY1 - bgHeight,
            (int)offsetX + screenWidth,
            drawY1,
            hImage,
            TRUE
        );
    }
}

