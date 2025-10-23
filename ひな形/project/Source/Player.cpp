#include "Player.h"
#include "Windows.h"
#include"Field.h"
#include"Shot.h"

static const float Gravity = 0.2;
static const float V0 = -7.3;
Player::Player()
{
	//画像読み込み
	/*if (-1 == LoadDivGraph("data/aoia.png", 12, 3, 4, 29, 40, hImage))
	{
		OutputDebugStringA("エラー発生");
	}*/
	width = 29;
	height = 40;

	//移動係数
	move = 1.0f;

	//横方向と縦方向のカウント数
	xcount = 0, ycount = 0;
	//添え字用変数
	ix = 0, iy = 0, result = 0;

	//初期位置
	x = 180;
	y = 400;

	//生きているかどうか
	life = true;

	//弾初期化
	memset(shot, 0, sizeof(shot));

	int temp = LoadGraph("data/stone.png");
	int w, h;
	GetGraphSize(temp, &w, &h);

	
	//グラフィックハンドルと画像のサイズを代入しとく
	for (int i = 0;i < PSHOT_NUM;i++)
	{
		shot[i].flag = false;
		shot[i].gh = temp;
		shot[i].width = w;
		shot[i].height = h;
	}
	count = 0;
}


Player::Player(int sx, int sy)
{
	hImage = LoadGraph("data/image/aoi.png");
	//画像読み込み
	/*if (-1 == LoadDivGraph("data/aoi.png", 12, 3, 4, 29, 40, hImage))
	{
		OutputDebugStringA("エラー発生");
	}*/
	width = 29;
	height = 40;

	//移動係数
	move = 1.0f;

	//横方向と縦方向のカウント数
	xcount = 0, ycount = 0;
	//添え字用変数
	ix = 0, iy = 0, result = 0;

	//初期位置
	x = sx;
	y = sy;
	shot(nullptr);

	time = 0;
	speed = 8;

	//生きているかどうか
	life = true;

	//弾初期化
	memset(shot, 0, sizeof(shot));

	int temp = LoadGraph("data/stone.png");
	int w, h;
	GetGraphSize(temp, &w, &h);


	//グラフィックハンドルと画像のサイズを代入しとく
	/*for (int i = 0;i < PSHOT_NUM;i++)
	{
		shot[i].flag = false;
		shot[i].gh = temp;
		shot[i].width = w;
		shot[i].height = h;
	}
	count = 0;*/
}

Player::~Player()
{
	// 生成したShotを破棄
	if (shot != nullptr) {
		delete shot;
		shot = nullptr;
	}
}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		x += speed;
		if (x > 640 - 64) x = 640 - 64;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		x -= speed;
		if (x < 0) x = 0;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		y -= speed;
		if (y < 0) x = 0;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		y += speed;
	}
}

void Player::Draw()
{
	//弾描画
	/*for (int i = 0;i < PSHOT_NUM;i++)
	{
		if (shot[i].flag)
		{
			DrawGraph(shot[i].x - shot[i].width / 2, shot[i].y - shot[i].height / 2, shot[i].gh, TRUE);
		}
	}*/

	
	time++;
	
	if (time > 63) time = 0;
	int viewType = time / 16;

	//生きていれば描画
	if (life)
		//描画
		DrawRectGraph(x, y, viewType * 64, 0, 64, 64, hImage, TRUE);
}

void Player::Shot()
{
	//キーが押されてかつ、6ループに一回発射
	if (CheckHitKey(KEY_INPUT_SPACE) == 1 && count % 6 == 0)
	{
		for (int i = 0;i < PSHOT_NUM;i++)
		{
			if (shot[i].flag == false)
			{
				shot[i].flag == true;
				shot[i].x = x;
				shot[i].y = y;
				break;
			}
		}
	}

	//弾を移動させる処理
	for (int i = 0;i < PSHOT_NUM;i++)
	{
		//発射している弾だけ
		if (shot[i].flag)
		{
			shot[i].y -= PSHOT_SPEED;

			//画面の外にはみ出したらフラグを戻す
			if (shot[i].y < -10)
			{
				shot[i].flag = false;
			}
		}
	}
}

void Player::All()
{
	Update();
	Shot();
	Draw();

	count++;
}
