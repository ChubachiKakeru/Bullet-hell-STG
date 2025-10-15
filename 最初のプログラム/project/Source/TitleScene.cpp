#include "TitleScene.h"
#include <DxLib.h>
#include "Screen.h"

int image;
const int CHR_SIZE = 64;
int coinImage;

TitleScene::TitleScene()
{
	image = LoadGraph("data/chara.png");
	coinImage = LoadGraph("data/item.png");
}

TitleScene::~TitleScene()
{
}

float a = 0; // TITLE SCENEを動かす
float x = 20; // キャラクターを動かす
float y = 50;
int pat = 4; // アニメーションのパターン
int count = 0; // アニメーションを遅らせるため
const int DOWN = 0;
const int LEFT = 1;
const int UP = 2;
const int RIGHT = 3;
int dir = DOWN; // 向き
const int STOP = 0;
const int WALK = 4;
int move = STOP;

float coinX = 0;
float coinY = 0;

int dispCount = 0; // 0より大きければ、COINGETと表示する
int dispX; // ゲットの表示する場所
int dispY;

int remainTime = 10*60;

//２つの引数を返す、ave()を作る

int add(int a, int b)
{
	int c = a + b;
	return c;
}

float ave(int a, int b)
{
	float c = (float)(a + b) / 2;
	c = ((float)a + (float)b)/2;
	return c;
}

void GoRight(float spd)
{
	x += spd;
	if (x > Screen::WIDTH - CHR_SIZE) {
		x = Screen::WIDTH - CHR_SIZE;
	}
	dir = RIGHT;
	move = WALK;
}

void TitleScene::Update()
{
	if (remainTime > 0) {
		remainTime -= 1;
	}
	float speed = 3;
	if (remainTime > 0) {
		move = STOP;
		if (CheckHitKey(KEY_INPUT_D)) { // Dキーを押したので、右に行く
			GoRight(speed);
		}
		if (CheckHitKey(KEY_INPUT_A)) {
			x -= speed;
			if (x < 0) {
				x = 0;
			}
			dir = LEFT;
			move = WALK;
		}
		if (CheckHitKey(KEY_INPUT_W)) {
			y -= speed;
			if (y < 0) {
				y = 0;
			}
			dir = UP;
			move = WALK;
		}
		if (CheckHitKey(KEY_INPUT_S)) {
			y += speed;
			if (y > Screen::HEIGHT - CHR_SIZE) {
				y = Screen::HEIGHT - CHR_SIZE;
			}
			dir = DOWN;
			move = WALK;
		}
	}
	count += 1;
	if (count >= 10) {
		count = 0;
		pat = (pat + 1) % 4 + move;
	}

	float dx = coinX - x; // X座標の差
	float dy = coinY - y; // Y座標の差
	float d = sqrt(dx*dx + dy*dy); // 距離を求める
	if (d<64) {
		dispCount = 60;
		dispX = coinX;
		dispY = coinY;
		// rand()は乱数(0～32767のどれかを取得)
		coinX = rand() % (Screen::WIDTH - CHR_SIZE);
		coinY = rand() % (Screen::HEIGHT - CHR_SIZE);
	}
	dispCount -= 1;
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	int x = add(3, 4);

	float a = ave(3, 5);

	DrawFormatString(400, 200, GetColor(255,255,255),
		"%d", x);
	DrawFormatString(500, 200, GetColor(255, 255, 255),
		"%f", a);
	// aの値を表示する
	DrawString(0, 0, "TITLE SCENE", GetColor(255, 255, 255), 0);
	DrawString(200, 400, "Push [P]Key To Play", GetColor(255, 255, 255));

	//	DrawGraph(0, 0, image, 1);
	DrawRectGraph(x, y, CHR_SIZE * pat, CHR_SIZE * dir,
		CHR_SIZE, CHR_SIZE, image, 1);

	DrawRectGraph(coinX, coinY, CHR_SIZE * 1, CHR_SIZE * 0,
		CHR_SIZE, CHR_SIZE, coinImage, 1);


	DrawFormatString(0, 100, GetColor(255,255,255),
		"X=%f Y=%f", x, y);

	if (dispCount > 0) {
		DrawFormatString(dispX, dispY, GetColor(255, 255, 255),
			"コインゲット！");
	}
	int second = remainTime/60;

	int nowSize = GetFontSize(); // 今のフォントサイズ
	SetFontSize(100); // サイズを変えて表示
	DrawFormatString(600, 0, GetColor(255, 255, 255),
		"TIME=%2d", second);
	SetFontSize(nowSize);

}
