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
}

void BackGround::Update() {
     //ゆっくりスクロール
    totalscrollY += scrollSpeed;
	scrollY += scrollSpeed;

     //背景画像の高さを超えたらループ
    int bgHeight = 1000;
   // GetGraphSize(hImage, nullptr, &bgHeight);

    if (scrollY >= bgHeight) {
        scrollY -= bgHeight;
        int temp = hImage;
        hImage = hImage2;
        hImage2 = temp;
    }
}

void BackGround::Draw() {
    if (hImage != -1) {
        int bgHeight = 1000;
       
        int drawY1 = -(int)scrollY + (int)offsetY;
        int drawY2 = drawY1 + bgHeight;

        // 1枚目
        DrawExtendGraph(
            (int)offsetX,           // 左上X
            drawY1,                 // 左上Y
            (int)offsetX + 840,     // 右下X（画面幅）
            drawY1 + 1000,           // 右下Y（画面高さ）
            hImage,
            TRUE
        );

        // 2枚目（ループ用）
        DrawExtendGraph(
            (int)offsetX,
            drawY2,
            (int)offsetX + 840,
            drawY2 + 1000,
            hImage2,
            TRUE
        );
    }
    
}

