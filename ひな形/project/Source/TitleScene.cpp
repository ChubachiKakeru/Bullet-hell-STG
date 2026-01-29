#include "TitleScene.h"
#include "../Library/SceneManager.h"
#include "DebugSceneSelector.h"
#include "StageSelectScene.h"

TitleScene::TitleScene()
    : m_titleImage(-1)
    , m_showDebugMenu(false)
    , m_debugKeyWait(0)
    , m_blinkTimer(0)
    , m_startPressed(false)
    , m_fadeOutTimer(0)
    , m_secretCommandIndex(0)      // ★追加★
    , m_secretKeyWait(0)            // ★追加★
    , m_secretUnlocked(false)       // ★追加★
{
    m_titleImage = LoadGraph("data/image/title.png");
    titleSoundHandle = LoadSoundMem(GAME_TITLE_SOUND_PATH);
    PlaySoundMem(titleSoundHandle, DX_PLAYTYPE_LOOP);
    DecisionSoundHandle = LoadSoundMem(GAME_DECISION_SOUND_PATH);
}

TitleScene::~TitleScene()
{
    if (m_titleImage != -1)
    {
        DeleteGraph(m_titleImage);
    }
    DeleteSoundMem(titleSoundHandle);
    DeleteSoundMem(DecisionSoundHandle);
}

// ★隠しコマンドチェック関数（追加）★
// コマンド: ↑↑↓↓←→ (上上下下左右)
void TitleScene::CheckSecretCommand()
{
    m_secretKeyWait++;

    // キー入力のクールダウン
    if (m_secretKeyWait < 10) return;

    // コマンド入力チェック
    bool keyPressed = false;

    if (m_secretCommandIndex == 0 || m_secretCommandIndex == 1) {
        // ↑（上）を2回
        if (CheckHitKey(KEY_INPUT_UP)) {
            m_secretCommandIndex++;
            keyPressed = true;
        }
    }
    else if (m_secretCommandIndex == 2 || m_secretCommandIndex == 3) {
        // ↓（下）を2回
        if (CheckHitKey(KEY_INPUT_DOWN)) {
            m_secretCommandIndex++;
            keyPressed = true;
        }
    }
    else if (m_secretCommandIndex == 4) {
        // ←（左）
        if (CheckHitKey(KEY_INPUT_LEFT)) {
            m_secretCommandIndex++;
            keyPressed = true;
        }
    }
    else if (m_secretCommandIndex == 5) {
        // →（右）
        if (CheckHitKey(KEY_INPUT_RIGHT)) {
            m_secretCommandIndex++;
            keyPressed = true;

            // ★コマンド完成！★
            if (m_secretCommandIndex >= SECRET_COMMAND_LENGTH) {
                m_secretUnlocked = true;
                printfDx("=== SECRET STAGE UNLOCKED! ===\n");
            }
        }
    }

    // 間違ったキーが押されたらリセット
    if (!keyPressed) {
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) ||
            CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_RIGHT)) {
            m_secretCommandIndex = 0;
            keyPressed = true;
        }
    }

    // キーが押されたらクールダウンリセット
    if (keyPressed) {
        m_secretKeyWait = 0;
    }
}

void TitleScene::Update()
{
    DebugSceneSelector::Update();

    m_debugKeyWait++;
    m_blinkTimer++;

    // ★隠しコマンドのチェック（Pキーが押される前のみ）★
    if (!m_startPressed && !m_showDebugMenu) {
        CheckSecretCommand();
    }

    // Dキーでデバッグメニューの表示切り替え
    if (m_debugKeyWait > 10 && CheckHitKey(KEY_INPUT_D))
    {
        m_showDebugMenu = !m_showDebugMenu;
        m_debugKeyWait = 0;
    }

    // ★Pキーが押されたらフェードアウト開始★
    if (!m_startPressed && CheckHitKey(KEY_INPUT_P))
    {
        PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);
        m_startPressed = true;
        m_fadeOutTimer = 0;
        StageSelectScene::ResetTotalTimer();
    }

    // ★隠しステージ解放時はSキーで直接ステージ3へ★
    if (m_secretUnlocked && !m_startPressed && CheckHitKey(KEY_INPUT_S))
    {
        StopSoundMem(titleSoundHandle);
        StageSelectScene::s_selectedStageNumber = 3;
        StageSelectScene::LoadStageData(3);
        StageSelectScene::ResetTotalTimer();
        SceneManager::ChangeScene("PLAY");
        return;
    }

    // ★フェードアウト中の処理★
    if (m_startPressed)
    {
        m_fadeOutTimer++;

        if (m_fadeOutTimer >= 60)
        {
            StopSoundMem(titleSoundHandle);
            SceneManager::ChangeScene("STAGESELECT");
            return;
        }
    }

    // デバッグメニュー
    if (m_showDebugMenu)
    {
        if (CheckHitKey(KEY_INPUT_1))
        {
            SceneManager::ChangeScene("STAGESELECT");
        }
        if (CheckHitKey(KEY_INPUT_2))
        {
            SceneManager::ChangeScene("SHOP");
        }
        if (CheckHitKey(KEY_INPUT_3))
        {
            SceneManager::ChangeScene("PLAY");
        }
        if (CheckHitKey(KEY_INPUT_4))
        {
            SceneManager::ChangeScene("CLEAR");
        }
        if (CheckHitKey(KEY_INPUT_5))
        {
            SceneManager::ChangeScene("GAMEOVER");
        }
        if (CheckHitKey(KEY_INPUT_6))
        {
            StageSelectScene::s_selectedStageNumber = 3;
            StageSelectScene::LoadStageData(3);
            SceneManager::ChangeScene("PLAY");
        }
    }
}

void TitleScene::Draw()
{
    const int screenWidth = 1280;
    const int screenHeight = 1280;

    if (m_titleImage != -1)
    {
        const int imageWidth = 877;
        const int imageHeight = 650;

        float scale = (float)screenHeight / imageHeight;

        int drawWidth = (int)(imageWidth * scale);
        int drawHeight = screenHeight;

        int drawX = (screenWidth - drawWidth) / 2;
        int drawY = 0;

        DrawExtendGraph(drawX, drawY, drawX + drawWidth, drawY + drawHeight, m_titleImage, TRUE);
    }
    else
    {
        DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 128), TRUE);
        DrawString(screenWidth / 2 - 50, screenHeight / 2 - 100, "GAME TITLE", GetColor(255, 255, 255));
    }

    int textStartY = 950;

    // ★「P: ゲーム開始」の描画★
    if (m_startPressed)
    {
        float progress = m_fadeOutTimer / 60.0f;
        float scale = 1.0f + progress * 0.5f;
        int alpha = (int)(255 * (1.0f - progress));

        int fontSize = (int)(32 * scale);
        SetFontSize(fontSize);

        int textWidth = GetDrawStringWidth("P: ゲーム開始", (int)strlen("P: ゲーム開始"));
        int centerX = screenWidth / 2 - textWidth / 2;
        int offsetY = (int)(-20 * progress);

        DrawString(centerX + 1, textStartY + offsetY + 1, "P: ゲーム開始", GetColor(0, 0, 0));
        DrawString(centerX - 1, textStartY + offsetY + 1, "P: ゲーム開始", GetColor(0, 0, 0));
        DrawString(centerX + 1, textStartY + offsetY - 1, "P: ゲーム開始", GetColor(0, 0, 0));
        DrawString(centerX - 1, textStartY + offsetY - 1, "P: ゲーム開始", GetColor(0, 0, 0));

        DrawString(centerX, textStartY + offsetY, "P: ゲーム開始", GetColor(200, alpha, 255));

        SetFontSize(16);
    }
    else
    {
        bool isVisible = (m_blinkTimer % 60) < 45;

        if (isVisible)
        {
            SetFontSize(32);

            int textWidth = GetDrawStringWidth("P: ゲーム開始", (int)strlen("P: ゲーム開始"));
            int centerX = screenWidth / 2 - textWidth / 2;

            DrawString(centerX + 1, textStartY + 1, "P: ゲーム開始", GetColor(0, 0, 0));
            DrawString(centerX - 1, textStartY + 1, "P: ゲーム開始", GetColor(0, 0, 0));
            DrawString(centerX + 1, textStartY - 1, "P: ゲーム開始", GetColor(0, 0, 0));
            DrawString(centerX - 1, textStartY - 1, "P: ゲーム開始", GetColor(0, 0, 0));
            DrawString(centerX, textStartY, "P: ゲーム開始", GetColor(255, 255, 255));

            SetFontSize(16);
        }
    }

    // ★隠しコマンド解放時の表示★
    if (m_secretUnlocked && !m_startPressed)
    {
        SetFontSize(28);

        int textWidth = GetDrawStringWidth("S: SECRET STAGE", (int)strlen("S: SECRET STAGE"));
        int centerX = screenWidth / 2 - textWidth / 2;
        int secretY = textStartY + 50;

        // 点滅エフェクト
        int blinkAlpha = ((m_blinkTimer / 15) % 2 == 0) ? 255 : 180;

        DrawString(centerX + 1, secretY + 1, "S: SECRET STAGE", GetColor(0, 0, 0));
        DrawString(centerX - 1, secretY + 1, "S: SECRET STAGE", GetColor(0, 0, 0));
        DrawString(centerX + 1, secretY - 1, "S: SECRET STAGE", GetColor(0, 0, 0));
        DrawString(centerX - 1, secretY - 1, "S: SECRET STAGE", GetColor(0, 0, 0));
        DrawString(centerX, secretY, "S: SECRET STAGE", GetColor(255, 255, blinkAlpha));

        SetFontSize(16);
    }

    // デバッグメニュー
    if (m_showDebugMenu)
    {
        DrawString(421, textStartY + 61, "=== デバッグメニュー ===", GetColor(0, 0, 0));
        DrawString(419, textStartY + 61, "=== デバッグメニュー ===", GetColor(0, 0, 0));
        DrawString(421, textStartY + 59, "=== デバッグメニュー ===", GetColor(0, 0, 0));
        DrawString(419, textStartY + 59, "=== デバッグメニュー ===", GetColor(0, 0, 0));
        DrawString(420, textStartY + 60, "=== デバッグメニュー ===", GetColor(255, 255, 0));

        DrawString(381, textStartY + 96, "1: ステージ選択", GetColor(0, 0, 0));
        DrawString(379, textStartY + 96, "1: ステージ選択", GetColor(0, 0, 0));
        DrawString(381, textStartY + 94, "1: ステージ選択", GetColor(0, 0, 0));
        DrawString(379, textStartY + 94, "1: ステージ選択", GetColor(0, 0, 0));
        DrawString(380, textStartY + 95, "1: ステージ選択", GetColor(0, 255, 255));

        DrawString(381, textStartY + 126, "2: ショップ", GetColor(0, 0, 0));
        DrawString(379, textStartY + 126, "2: ショップ", GetColor(0, 0, 0));
        DrawString(381, textStartY + 124, "2: ショップ", GetColor(0, 0, 0));
        DrawString(379, textStartY + 124, "2: ショップ", GetColor(0, 0, 0));
        DrawString(380, textStartY + 125, "2: ショップ", GetColor(0, 255, 255));

        DrawString(381, textStartY + 156, "3: プレイシーン", GetColor(0, 0, 0));
        DrawString(379, textStartY + 156, "3: プレイシーン", GetColor(0, 0, 0));
        DrawString(381, textStartY + 154, "3: プレイシーン", GetColor(0, 0, 0));
        DrawString(379, textStartY + 154, "3: プレイシーン", GetColor(0, 0, 0));
        DrawString(380, textStartY + 155, "3: プレイシーン", GetColor(0, 255, 255));

        DrawString(381, textStartY + 186, "4: クリアシーン", GetColor(0, 0, 0));
        DrawString(379, textStartY + 186, "4: クリアシーン", GetColor(0, 0, 0));
        DrawString(381, textStartY + 184, "4: クリアシーン", GetColor(0, 0, 0));
        DrawString(379, textStartY + 184, "4: クリアシーン", GetColor(0, 0, 0));
        DrawString(380, textStartY + 185, "4: クリアシーン", GetColor(0, 255, 255));

        DrawString(381, textStartY + 216, "5: ゲームオーバー", GetColor(0, 0, 0));
        DrawString(379, textStartY + 216, "5: ゲームオーバー", GetColor(0, 0, 0));
        DrawString(381, textStartY + 214, "5: ゲームオーバー", GetColor(0, 0, 0));
        DrawString(379, textStartY + 214, "5: ゲームオーバー", GetColor(0, 0, 0));
        DrawString(380, textStartY + 215, "5: ゲームオーバー", GetColor(0, 255, 255));

        DrawString(381, textStartY + 246, "6: ステージ3 (直接)", GetColor(0, 0, 0));
        DrawString(379, textStartY + 246, "6: ステージ3 (直接)", GetColor(0, 0, 0));
        DrawString(381, textStartY + 244, "6: ステージ3 (直接)", GetColor(0, 0, 0));
        DrawString(379, textStartY + 244, "6: ステージ3 (直接)", GetColor(0, 0, 0));
        DrawString(380, textStartY + 245, "6: ステージ3 (直接)", GetColor(0, 255, 255));
    }

    DebugSceneSelector::Draw();
}