#include "Player.h"
#include "Field.h"
#include "playerBullet.h"
#include "Bomb.h"
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

    // ===== SEロード =====
    playerHitSoundHandle = LoadSoundMem(GAME_PHIT_SOUND_PATH);
    playerShotSoundHandle = LoadSoundMem(GAME_PSHOT_SOUND_PATH);
    playerBombSoundHandle = LoadSoundMem(GAME_PBOMB_SOUND_PATH);


    x = 300;
    y = 900;
    velocity = 0;
    onGround = false;
    shotTimer = 0.0f;
    hp = INITIAL_HP;
    size = 50.0f;

    isActive = true;

    // ボム関連
    bombCount = 10;
    bombSize = BOMB_SIZE;
    bombSpeed = BOMB_SPEED;
    prevBombKeyPressed = false;
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
    DeleteSoundMem(playerHitSoundHandle);
    DeleteSoundMem(playerShotSoundHandle);
    DeleteSoundMem(playerBombSoundHandle);
}

// ========================================
// ダメージ処理
// ========================================
void Player::TakeDamage(int damage)
{
    hp -= damage;
    PlaySoundMem(playerHitSoundHandle, DX_PLAYTYPE_BACK);
    if (hp <= 0) {
        hp = 0;
        StopSoundFile();  // ★BGM停止（1回）
        // ゲームオーバーシーンに遷移
        SceneManager::ChangeScene("GAMEOVER");
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

    PlaySoundMem(playerShotSoundHandle, DX_PLAYTYPE_BACK);
}

// ========================================
// ボム発射（変わり種弾として扱う）
// ========================================
void Player::ShootBomb()
{
    if (bombCount <= 0) return;

    bombCount--;

    // ボムとして自キャラの上方向に発射
    float startX = x + size / 2.0f - BOMB_SIZE / 2.0f;
    float startY = y - BOMB_SIZE;

    float bombVY = -8.0f; // 上方向速度
    float bombVX = 0.0f;

    // Bombクラスとして生成
    new Bomb(startX, startY, bombVX, bombVY, BOMB_SIZE);

    PlaySoundMem(playerBombSoundHandle, DX_PLAYTYPE_BACK);
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

    // ボム発射（キーが押された瞬間のみ発射）
    bool currentBombKeyPressed = CheckHitKey(KEY_INPUT_J);
    if (currentBombKeyPressed && !prevBombKeyPressed) {
        ShootBomb();
    }
    prevBombKeyPressed = currentBombKeyPressed;
}

// ========================================
// 描画
// ========================================
void Player::Draw()
{
    DrawGraph((int)x, (int)y, hImage, TRUE);
    Field* field = FindGameObject<Field>();

    if (isActive) {
        // 当たり判定の円(水色)
        DrawCircle((float)x + 78, (float)y + 50, (int)BULLET_RADIUS + 10.0f, GetColor(0, 255, 255), FALSE);
    }
    DrawFormatString(1000, 270, GetColor(0, 255, 255), "=== PLAYER ===");
    DrawFormatString(1000, 300, GetColor(0, 255, 255), "PLAYER HP: %d", hp);
    DrawFormatString(1000, 330, GetColor(0, 255, 255), "BOMB: %d", bombCount);
}