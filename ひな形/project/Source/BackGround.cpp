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
        //GetGraphSize(hImage,nullptr,  &bgHeight);

         //2枚並べて描画（ループ用）
        /*DrawGraph(0, -(int)scrollY, hImage, TRUE);
        DrawGraph(0, -(int)scrollY + bgHeight, hImage2, TRUE);*/
        int drawY1 = -(int)scrollY + (int)offsetY;
        int drawY2 = drawY1 + bgHeight;

        // 1枚目
        DrawExtendGraph(
            (int)offsetX,           // 左上X
            drawY1,                 // 左上Y
            (int)offsetX + 640,     // 右下X（画面幅）
            drawY1 + 1000,           // 右下Y（画面高さ）
            hImage,
            TRUE
        );

        // 2枚目（ループ用）
        DrawExtendGraph(
            (int)offsetX,
            drawY2,
            (int)offsetX + 640,
            drawY2 + 1000,
            hImage2,
            TRUE
        );
    }
    
}

//int BackGround::HitCheckLeft(int px, int py)
//{
//    if (py < 400)
//        return 0;
//    int x = px / 64;
//    int y = (py - 400) / 64;
//    if (maps[y][x] == 1)
//        return 64 - px % 64;
//    return 0;
//}



//BackGround::BackGround() : GameObject() {
//    scrollY = 0;
//    scrollSpeed = 1;   //さらにゆっくり
//}
//
//BackGround::~BackGround() {
//}
//
//void BackGround::Update() {
//    scrollY += scrollSpeed;
//
//     //600ピクセルでループ
//    if (scrollY >= 600) {
//        scrollY = 0;
//    }
//}
//
//void BackGround::Draw() {
//     //濃い青の背景
//    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 30), TRUE);
//
//     //小さい星（50個）
//    for (int i = 0; i < 50; i++) {
//        int starX = (i * 73) % 1280;
//        int starY = ((i * 127 + scrollY) % 800);
//
//        if (starY < 800) {
//            DrawCircle(starX, (int)starY, 1, GetColor(255, 255, 255), TRUE);
//        }
//    }
//
//     //大きい星（20個）
//    for (int i = 0; i < 20; i++) {
//        int starX = (i * 113) % 1280;
//        int starY = ((i * 179 + scrollY * 2) % 800);
//
//        if (starY < 800) {
//            DrawCircle(starX, (int)starY, 2, GetColor(255, 255, 200), TRUE);
//        }
//    }
//}

