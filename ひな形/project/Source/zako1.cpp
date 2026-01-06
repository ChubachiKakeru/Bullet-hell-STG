#include "zako1.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include <cmath>

namespace {
    constexpr float SHOT_INTERVAL = 120.0f;
    constexpr float PATTERN_TIME = 300.0f;
    constexpr float SPEED_NORMAL = 1.5f;
    constexpr float SPEED_SLOW = 0.8f;
    constexpr float MOVE_SPEED = 2.0f;
    constexpr float RADIUS = 20.0f;
    constexpr float EDGE_MARGIN = 100.0f;
}

zako1::zako1(int sx, int sy, Zako1Pattern pat) : Enemy(),
hImage(LoadGraph("data/image/file/chara/enemy.png")),
x(sx), y(sy), hp(1), isActive(true), patternComplete(false),
pattern(pat), moveTimer(0), shotTimer(0), movePhase(0) {
}

zako1::~zako1() { if (hImage != -1) DeleteGraph(hImage); }

void zako1::Update()
{
    if (!isActive) return;

    moveTimer++;
    shotTimer++;

    // 移動（下には動かない）
    if (pattern == Zako1Pattern::PATTERN_3) {
        // パターン3のみ左右移動
        x += (movePhase == 0 ? MOVE_SPEED : -MOVE_SPEED);
        float edge = (movePhase == 0 ? Field::STAGE_RIGHT : Field::STAGE_LEFT);
        float limit = edge + (movePhase == 0 ? -EDGE_MARGIN : EDGE_MARGIN);

        if ((movePhase == 0 && x >= limit) || (movePhase == 1 && x <= limit)) {
            x = limit;
            if (movePhase == 0) movePhase = 1;
            else patternComplete = true;
        }
    }

    // パターン完了判定（パターン1, 2）
    if (pattern != Zako1Pattern::PATTERN_3) {
        if (moveTimer >= PATTERN_TIME) patternComplete = true;
    }

    // 弾発射
    if (shotTimer >= SHOT_INTERVAL) {
        ShootBullet();
        shotTimer = 0;
    }

    // 画面外判定（念のため）
    if (y > Field::STAGE_BOTTOM + 200 || x < Field::STAGE_LEFT - 200 || x > Field::STAGE_RIGHT + 200) {
        isActive = false;
    }
}

void zako1::ShootBullet()
{
    if (pattern != Zako1Pattern::PATTERN_3) {
        new enemyBullet(x, y, 0, 5, 8);
        return;
    }

    Player* p = FindGameObject<Player>();
    if (!p) return;

    float dx = p->GetX() - x, dy = p->GetY() - y;
    float len = sqrt(dx * dx + dy * dy);
    if (len > 0) { dx /= len; dy /= len; }

    for (float ang : {0.0f, 0.3f, -0.3f, 0.5f, -0.5f}) {
        float c = cos(ang), s = sin(ang);
        new enemyBullet(x, y, (dx * c - dy * s) * 5, (dx * s + dy * c) * 5, 8);
    }
}

void zako1::Draw()
{
    if (isActive && hImage != -1) DrawGraph(x - 16, y - 16, hImage, TRUE);

    // デバッグ用：当たり判定を表示
    if (isActive) {
        DrawCircle(x, y, RADIUS, GetColor(255, 0, 0), FALSE);
        DrawFormatString(x - 20, y - 30, GetColor(255, 255, 255), "ZAKO");
    }
}

void zako1::TakeDamage(int damage)
{
    if (!isActive) return;
    hp -= damage;
    if (hp <= 0) { hp = 0; isActive = false; }
}

bool zako1::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;
    float dx = bx - x, dy = by - y;
    if (sqrt(dx * dx + dy * dy) < RADIUS + rad) {
        TakeDamage(1);
        return true;
    }
    return false;
}