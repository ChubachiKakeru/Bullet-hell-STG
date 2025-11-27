#include "BootScene.h"
#include"Common.h"

BootScene::BootScene()
{
	new Common();
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TITLE"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
