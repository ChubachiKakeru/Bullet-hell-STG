#include "BackGround.h"
#include "DxLib.h"

//BackGround::BackGround() : GameObject() {
//    hImage = LoadGraph("data/image/Background.png");
//    scrollY = 0.0f;
//    scrollSpeed = 0.5f;   //�������i0.5�s�N�Z��/�t���[���j
//}
//
//BackGround::~BackGround() {
//    if (hImage != -1) {
//        DeleteGraph(hImage);
//    }
//}
//
//void BackGround::Update() {
//     //�������X�N���[��
//    scrollY += scrollSpeed;
//
//     //�w�i�摜�̍����𒴂����烋�[�v
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
//         //2�����ׂĕ`��i���[�v�p�j
//        DrawGraph(0, -(int)scrollY, hImage, TRUE);
//        DrawGraph(0, -(int)scrollY + bgHeight, hImage, TRUE);
//    }
//}


BackGround::BackGround() : GameObject() {
    scrollY = 0;
    scrollSpeed = 1;   //����ɂ������
}

BackGround::~BackGround() {
}

void BackGround::Update() {
    scrollY += scrollSpeed;

     //600�s�N�Z���Ń��[�v
    if (scrollY >= 600) {
        scrollY = 0;
    }
}

void BackGround::Draw() {
     //�Z���̔w�i
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 30), TRUE);

     //���������i50�j
    for (int i = 0; i < 50; i++) {
        int starX = (i * 73) % 1280;
        int starY = ((i * 127 + scrollY) % 800);

        if (starY < 800) {
            DrawCircle(starX, (int)starY, 1, GetColor(255, 255, 255), TRUE);
        }
    }

     //�傫�����i20�j
    for (int i = 0; i < 20; i++) {
        int starX = (i * 113) % 1280;
        int starY = ((i * 179 + scrollY * 2) % 800);

        if (starY < 800) {
            DrawCircle(starX, (int)starY, 2, GetColor(255, 255, 200), TRUE);
        }
    }
}

