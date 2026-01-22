// StageSelectScene.cpp
#include "StageSelectScene.h"
#include "../Library/SceneManager.h"
#include "DxLib.h"

// 静的メンバの初期化
int StageSelectScene::s_selectedStageNumber = 1;
Stage1Data StageSelectScene::s_stage1;
Stage2Data StageSelectScene::s_stage2;
StageData* StageSelectScene::s_currentStageData = nullptr;
int StageSelectScene::s_totalGameTimer = 0;  // ★追加★

StageSelectScene::StageSelectScene()
    : m_backgroundImage(-1)
    , m_selectedStage(0)
    , m_keyWait(0)
{
    m_backgroundImage = LoadGraph("Graphics/StageSelect.png");
    // 前回の選択を復元（1-indexedから0-indexedに変換）
    m_selectedStage = s_selectedStageNumber - 1;
    // ★キー入力を受け付けない期間を設定★
    m_keyWait = -60; // 60フレーム（1秒）待機

    // ===== SEロード =====
    CancelSoundHandle = LoadSoundMem(GAME_CANCEL_SOUND_PATH);
    CusorSoundHandle = LoadSoundMem(GAME_CURSOR_SOUND_PATH);
    DecisionSoundHandle = LoadSoundMem(GAME_DECISION_SOUND_PATH);
}

StageSelectScene::~StageSelectScene()
{
    if (m_backgroundImage != -1)
    {
        DeleteGraph(m_backgroundImage);
    }

    DeleteSoundMem(CancelSoundHandle);
    DeleteSoundMem(CusorSoundHandle);
    //DeleteSoundMem(DecisionSoundHandle);
}

void StageSelectScene::LoadStageData(int stageNumber)
{
    switch (stageNumber)
    {
    case 1:
        if (!s_stage1.LoadFromCSV("data/stage1.csv"))
        {
            printfDx("Stage1 CSV not found, using hardcoded data\n");
            s_stage1.Initialize();
        }
        s_currentStageData = &s_stage1;
        break;

    case 2:
        if (!s_stage2.LoadFromCSV("data/stage2.csv"))
        {
            printfDx("Stage2 CSV not found, using hardcoded data\n");
            s_stage2.Initialize();
        }
        s_currentStageData = &s_stage2;
        break;

    default:
        printfDx("Invalid stage number: %d\n", stageNumber);
        s_currentStageData = nullptr;
        break;
    }
}

// 次のステージに進む（ステージクリア時に呼ぶ）
void StageSelectScene::GoToNextStage()
{
    // ★現在のステージ番号を保存してから増やす★
    int previousStage = s_selectedStageNumber;
    s_selectedStageNumber++;

    // ★ステージ1クリア後（previousStageが1）はショップへ★
    if (previousStage == 1)
    {
        // 次のステージ（ステージ2）のデータを読み込む
        LoadStageData(s_selectedStageNumber);
        // ショップシーンへ遷移
        SceneManager::ChangeScene("SHOP");
        return;
    }

    // 最後のステージを超えたら1に戻す（またはゲームクリア画面へ）
    if (s_selectedStageNumber > STAGE_COUNT)
    {
        s_selectedStageNumber = 1; // ループする場合
        // または SceneManager::ChangeScene("GAMECLEAR"); // ゲームクリア画面がある場合
    }

    // 次のステージデータを読み込む
    LoadStageData(s_selectedStageNumber);

    // プレイシーンへ遷移
    SceneManager::ChangeScene("PLAY");
}

void StageSelectScene::Update()
{
    m_keyWait++;

    // ★キー入力待機中は何もしない★
    if (m_keyWait < 0)
    {
        return;
    }

    // キー入力の受付（連続入力防止）
    if (m_keyWait > 10)
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            m_selectedStage = (m_selectedStage - 1 + STAGE_COUNT) % STAGE_COUNT;
            m_keyWait = 0;
            //PlaySoundMem(/* カーソル移動音 */, DX_PLAYTYPE_BACK);
            PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            m_selectedStage = (m_selectedStage + 1) % STAGE_COUNT;
            m_keyWait = 0;
            //PlaySoundMem(/* カーソル移動音 */, DX_PLAYTYPE_BACK);
            PlaySoundMem(CusorSoundHandle, DX_PLAYTYPE_BACK);
        }
    }
    // 決定キー
    if (m_keyWait > 10 && (CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_RETURN)))
    {
        PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);

        // 選択されたステージ番号を保存（0-indexedから1-indexedに変換）
        s_selectedStageNumber = m_selectedStage + 1;

        // ★ステージ1を選んだ場合のみタイマーをリセット★
        if (s_selectedStageNumber == 1)
        {
            s_totalGameTimer = 0;
        }

        // ステージデータを読み込む
        LoadStageData(s_selectedStageNumber);
        //PlaySoundMem(/* 決定音 */, DX_PLAYTYPE_BACK);
        PlaySoundMem(DecisionSoundHandle, DX_PLAYTYPE_BACK);

        // ★カウントダウンを削除し、即座にPlaySceneへ遷移★
        SceneManager::ChangeScene("PLAY");
    }
    // キャンセルキー（Oキーでタイトルへ）
    if (CheckHitKey(KEY_INPUT_O))
    {
        //PlaySoundMem(/* キャンセル音 */, DX_PLAYTYPE_BACK);
        PlaySoundMem(CancelSoundHandle, DX_PLAYTYPE_BACK);
        SceneManager::ChangeScene("TITLE");
    }
}

void StageSelectScene::Draw()
{
    // 背景描画
    if (m_backgroundImage != -1)
    {
        DrawGraph(0, 0, m_backgroundImage, FALSE);
    }
    else
    {
        DrawBox(0, 0, 1280, 720, GetColor(50, 50, 100), TRUE);
    }

    // タイトル
    SetFontSize(48);
    DrawString(450, 100, "=== ステージ選択 ===", GetColor(255, 255, 255));

    // ステージリスト（2ステージのみ）
    SetFontSize(36);
    for (int i = 0; i < STAGE_COUNT; i++)
    {
        int yPos = 300 + i * 80; // 位置を調整

        // 選択中のステージはハイライト
        if (i == m_selectedStage)
        {
            // カーソル表示
            DrawString(480, yPos, "＞", GetColor(255, 255, 0));
            // 選択枠
            DrawBox(500, yPos - 5, 780, yPos + 45, GetColor(255, 255, 0), FALSE);
            // テキスト（黄色）
            DrawFormatString(520, yPos, GetColor(255, 255, 0), "ステージ %d", i + 1);
        }
        else
        {
            // 非選択時（グレー）
            DrawFormatString(520, yPos, GetColor(180, 180, 180), "ステージ %d", i + 1);
        }
    }

    // 操作説明
    SetFontSize(24);
    DrawString(400, 620, "↑↓キーで選択 / SPACE/ENTER: 決定 / O: タイトルへ",
        GetColor(200, 200, 200));
    SetFontSize(16); // フォントサイズを元に戻す
}