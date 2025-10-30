#pragma once
#include"Field.h"
#include "../Library/GameObject.h"

class Player;
class Bulett;

enum class BossPattern {
	CIRCLE,       // �~�O��
	FIGURE_EIGHT, // ���̎�
	LEFT_RIGHT    // ���E�ړ�
};

class Boss1 : public GameObject
{
public:
	Boss1();
	Boss1(int sx, int sy);
	~Boss1();
	void Update()override;
	void Draw() override;

	bool IsActive() const { return isActive; }
	int GetHP() const { return hp; }
	void TakeDamage(int damage);
private:
	int hImage;
	float x, y;
    float centerX, centerY;
    int hp;
    bool isActive;

    // �ړ��p�^�[���֘A
    BossPattern pattern;
    float moveTimer;
    float patternChangeTime;
    float patternTimer;

    // �~�O���p
    float radius;
    float angle;
    float angularSpeed;

    // ���̎��p
    float figureEightScale;

    // ���E�ړ��p
    float speed;
    int direction;

    // �e���ˊ֘A
    float shotTimer;
    float shotInterval;

    void UpdateCircle();
    void UpdateFigureEight();
    void UpdateLeftRight();
    void ChangePattern();
    void ShootBullet();
};

