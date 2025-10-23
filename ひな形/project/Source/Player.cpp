#include "Player.h"
#include "Windows.h"
#include"Field.h"
#include"Shot.h"

static const float Gravity = 0.2;
static const float V0 = -7.3;
Player::Player()
{
	//�摜�ǂݍ���
	/*if (-1 == LoadDivGraph("data/aoia.png", 12, 3, 4, 29, 40, hImage))
	{
		OutputDebugStringA("�G���[����");
	}*/
	width = 29;
	height = 40;

	//�ړ��W��
	move = 1.0f;

	//�������Əc�����̃J�E���g��
	xcount = 0, ycount = 0;
	//�Y�����p�ϐ�
	ix = 0, iy = 0, result = 0;

	//�����ʒu
	x = 180;
	y = 400;

	//�����Ă��邩�ǂ���
	life = true;

	//�e������
	memset(shot, 0, sizeof(shot));

	int temp = LoadGraph("data/stone.png");
	int w, h;
	GetGraphSize(temp, &w, &h);

	
	//�O���t�B�b�N�n���h���Ɖ摜�̃T�C�Y�������Ƃ�
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
	//�摜�ǂݍ���
	/*if (-1 == LoadDivGraph("data/aoi.png", 12, 3, 4, 29, 40, hImage))
	{
		OutputDebugStringA("�G���[����");
	}*/
	width = 29;
	height = 40;

	//�ړ��W��
	move = 1.0f;

	//�������Əc�����̃J�E���g��
	xcount = 0, ycount = 0;
	//�Y�����p�ϐ�
	ix = 0, iy = 0, result = 0;

	//�����ʒu
	x = sx;
	y = sy;
	shot(nullptr);

	time = 0;
	speed = 8;

	//�����Ă��邩�ǂ���
	life = true;

	//�e������
	memset(shot, 0, sizeof(shot));

	int temp = LoadGraph("data/stone.png");
	int w, h;
	GetGraphSize(temp, &w, &h);


	//�O���t�B�b�N�n���h���Ɖ摜�̃T�C�Y�������Ƃ�
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
	// ��������Shot��j��
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
	//�e�`��
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

	//�����Ă���Ε`��
	if (life)
		//�`��
		DrawRectGraph(x, y, viewType * 64, 0, 64, 64, hImage, TRUE);
}

void Player::Shot()
{
	//�L�[��������Ă��A6���[�v�Ɉ�񔭎�
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

	//�e���ړ������鏈��
	for (int i = 0;i < PSHOT_NUM;i++)
	{
		//���˂��Ă���e����
		if (shot[i].flag)
		{
			shot[i].y -= PSHOT_SPEED;

			//��ʂ̊O�ɂ͂ݏo������t���O��߂�
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
