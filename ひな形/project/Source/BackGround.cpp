#include "BackGround.h"
#include "DxLib.h"

//BackGround::BackGround() : GameObject() {
//    hImage = LoadGraph("data/image/Background.png");
//    scrollY = 0.0f;
//    scrollSpeed = 0.5f;   //ゆっくり（0.5ピクセル/フレーム）
//}
//
//BackGround::~BackGround() {
//    if (hImage != -1) {
//        DeleteGraph(hImage);
//    }
//}
//
//void BackGround::Update() {
//     //ゆっくりスクロール
//    scrollY += scrollSpeed;
//
//     //背景画像の高さを超えたらループ
//    int bgHeight = 0;
//    GetGraphSize(hImage, nullptr, &bgHeight);
//
//    if (scrollY >= bgHeight) {
//        scrollY = 0.0f;
//    }
//}
//
//void BackGround::Draw() {
//    if (hImage != -1) {
//        int bgHeight = 0;
//        GetGraphSize(hImage, nullptr, &bgHeight);
//
//         //2枚並べて描画（ループ用）
//        DrawGraph(0, -(int)scrollY, hImage, TRUE);
//        DrawGraph(0, -(int)scrollY + bgHeight, hImage, TRUE);
//    }
//}


BackGround::BackGround() : GameObject() {
    scrollY = 0;
    scrollSpeed = 1;   //さらにゆっくり
}

BackGround::~BackGround() {
}

void BackGround::Update() {
    scrollY += scrollSpeed;

     //600ピクセルでループ
    if (scrollY >= 600) {
        scrollY = 0;
    }
}

void BackGround::Draw() {
     //濃い青の背景
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 30), TRUE);

     //小さい星（50個）
    for (int i = 0; i < 50; i++) {
        int starX = (i * 73) % 1280;
        int starY = ((i * 127 + scrollY) % 800);

        if (starY < 800) {
            DrawCircle(starX, (int)starY, 1, GetColor(255, 255, 255), TRUE);
        }
    }

     //大きい星（20個）
    for (int i = 0; i < 20; i++) {
        int starX = (i * 113) % 1280;
        int starY = ((i * 179 + scrollY * 2) % 800);

        if (starY < 800) {
            DrawCircle(starX, (int)starY, 2, GetColor(255, 255, 200), TRUE);
        }
    }
}

