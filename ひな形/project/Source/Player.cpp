#include "Player.h"
#include "Field.h"
#include "playerBullet.h"
#include "enemyBullet.h"
#include "EnemyBullet2.h"
#include "Screen.h"
#include "Common.h"
#include <cmath>

// ========================================
// 定数
// ========================================
namespace {
    constexpr float MOVE_SPEED = 7.0f;
    constexpr float SHOT_COOLDOWN = 50.0f;
    constexpr int INITIAL_HP = 100;
    constexpr float PLAYER_COLLISION_RADIUS = 30.0f;
    constexpr float PLAYER_CENTER_OFFSET = 60.0f;
    constexpr float BULLET_SPEED = -10.0f;
    constexpr float BULLET_RADIUS = 8.0f;

    constexpr float BOMB_SIZE = 150.0f;
    constexpr float BOMB_SPEED = -10.0f; // 上方向
}

// ========================================
// コンストラクタ / デストラクタ
// ========================================
Player::Player() : GameObject()
{
    hImage = LoadGraph("data/image/file/chara/player.png");
    hBombImage = LoadGraph("data/image/bomb.png");

    x = 300;
    y = 900;
    velocity = 0;
    onGround = false;
    shotTimer = 0.0f;
    hp = INITIAL_HP;
    size = 50.0f;

    isActive = true;

    // ボム関連
    bombCount = 3;
    bombSize = BOMB_SIZE;
    bombSpeed = BOMB_SPEED;
}

Player::Player(int sx, int sy) : Player()
{
    x = sx;
    y = sy;
}

Player::~Player()
{
    if (hImage != -1) DeleteGraph(hImage);
    if (hBombImage != -1) DeleteGraph(hBombImage);
}

// ========================================
// ダメージ処理
// ========================================
void Player::TakeDamage(int damage)
{
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        SceneManager::ChangeScene("TITLE");
    }
}

// ========================================
// 当たり判定
// ========================================
bool Player::IsHit(float bx, float by, int rad)
{
    float dx = bx - (x + PLAYER_CENTER_OFFSET);
    float dy = by - (y + PLAYER_CENTER_OFFSET);
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < PLAYER_COLLISION_RADIUS + rad) {
        TakeDamage(0);
        return true;
    }
    return false;
}

// ========================================
// 弾発射
// ========================================
void Player::ShootBullet()
{
    int bulletX = (int)x + 50 / 2;
    int bulletY = (int)y + 50 / 2;
    new playerBullet(bulletX, bulletY, 0, BULLET_SPEED, BULLET_RADIUS);
}

// ========================================
// ボム発射（変わり種弾として扱う）
// ========================================
void Player::ShootBomb()
{
    if (bombCount <= 0) return;

    bombCount--;

    // 弾として自キャラの上方向に発射（中心を自キャラの上）
    float startX = x + size / 2.0f - BOMB_SIZE / 2.0f;
    float startY = y - BOMB_SIZE; // 自キャラの前に配置

    // 弾の大きさ固定150x150
    float bombVY = -8.0f; // 上方向速度
    float bombVX = 0.0f;

    // 敵弾に当たると消える処理は enemyBullet 側で判定する想定
    new enemyBullet(startX, startY, bombVX, bombVY, BOMB_SIZE, 0);
}

// ========================================
// 更新処理
// ========================================
void Player::Update()
{
    Field* field = FindGameObject<Field>();
    if (!field) return;

    // 移動
    if (CheckHitKey(KEY_INPUT_D)) x += MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_A)) x -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_W)) y -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_S)) y += MOVE_SPEED;

    // ステージ境界内に制限（画面の青い部分のみ） 
     if (x < Field::STAGE_LEFT) x = Field::STAGE_LEFT; 
     if (x > Field::STAGE_RIGHT - 50 / 2) x = Field::STAGE_RIGHT - 50 / 2;
     // プレイヤー幅を考慮 
     if (y < Field::STAGE_TOP) y = Field::STAGE_TOP; 
     if (y > Screen::HEIGHT - 100) y = Screen::HEIGHT - 100; // 画面下端まで移動可能

    // 弾発射
    shotTimer += 1.0f;
    if (CheckHitKey(KEY_INPUT_H) && shotTimer >= SHOT_COOLDOWN) {
        ShootBullet();
        shotTimer = 0.0f;
    }

    // ボム発射
    if (CheckHitKey(KEY_INPUT_J)) {
        ShootBomb();
    }
}

// ========================================
// 描画
// ========================================
void Player::Draw()
{
    DrawGraph((int)x, (int)y, hImage, TRUE);
    // ボムは弾として敵弾を消すので描画は enemyBullet に任せる
}
