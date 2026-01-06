#include "Boss2.h"
#include"Field.h"
#include"enemyBullet.h"
#include"Player.h"
#include"Boss1.h"
#include"SceneFactory.h"
#include <cmath>


Boss2::Boss2() : GameObject()
{
    hImage = LoadGraph("data/image/file/chara/boss2.png");
    x = 300.0f;
    y = 200.0f;
    centerX = 300.0f;
    centerY = 200.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase2 = BulletPhase2::PHASE_1;
    previousPhase2 = BulletPhase2::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;
}

Boss2::Boss2(int sx, int sy)
{
    hImage = LoadGraph("data/image/file/chara/boss2.png");
    x = (float)sx;
    y = (float)sy;
    centerX = (float)sx;
    centerY = (float)sy;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase2 = BulletPhase2::PHASE_1;
    previousPhase2 = BulletPhase2::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;
}

Boss2::~Boss2()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
    }
}

void Boss2::Update()
{
    if (!isActive) return;

    // HPベースのフェーズチェック
    CheckPhaseTransition();

    // 弾発射タイマー更新
    shotTimer += 1.0f;

    // 各フェーズの更新
    switch (bulletPhase2) {
    case BulletPhase2::PHASE_1:
        UpdatePhase1();
        break;
    case BulletPhase2::PHASE_2:
        UpdatePhase2();
        break;
    case BulletPhase2::PHASE_3:
        UpdatePhase3();
        break;
    }

    // 弾発射判定
    if (shotTimer >= shotInterval) {
        switch (bulletPhase2) {
        case BulletPhase2::PHASE_1:
            ShotBullet(45.0f, 8.0f);
            break;
        case BulletPhase2::PHASE_2:
            ShotBullet(30.0f, 3.0f);
            break;
        case BulletPhase2::PHASE_3:
            if (!isCharging) {
                ShotBullet(25.0f, 5.0f);
            }
            break;
        }
        shotTimer = 0.0f;
    }
}

void Boss2::CheckPhaseTransition()
{
    previousPhase2 = bulletPhase2;

    if (currentHp <= Phase3Hp) {
        bulletPhase2 = BulletPhase2::PHASE_3;
    }
    else if (currentHp <= Phase2Hp) {
        bulletPhase2 = BulletPhase2::PHASE_2;
    }
    else {
        bulletPhase2 = BulletPhase2::PHASE_1;
    }

    if (bulletPhase2 != previousPhase2) {
        OnPhaseChanged(GetCurrentPhaseNumber());
    }
}

void Boss2::OnPhaseChanged(int newPhase)
{
    printfDx("フェーズ %d に移行！HP: %d\n", newPhase, currentHp);

    // フェーズごとの初期化
    switch (bulletPhase2) {
    case BulletPhase2::PHASE_1:
        x = 400.0f;
        y = 100.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase2::PHASE_2:
        x = 100.0f;
        y = 100.0f;
        moveDirection = 2.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase2::PHASE_3:
        x = 400.0f;
        y = 100.0f;
        shotInterval = 60.0f;
        isCharging = false;
        chargeTimer = 0.0f;
        break;
    }
}

void Boss2::UpdatePhase1()
{

}

void Boss2::UpdatePhase2()
{

}

void Boss2::UpdatePhase3()
{

}

void Boss2::ShotBullet(float angle, float num)
{

}

void Boss2::Draw() {

}

int Boss2::GetCurrentPhaseNumber() const
{
    return (int)bulletPhase2 + 1;
}