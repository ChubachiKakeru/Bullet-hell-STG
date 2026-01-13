#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "BootScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "GameOverScene.h"
#include "StageSelectScene.h"
#include "ShopScene.h"
#include "ClearScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new BootScene();
}

SceneBase* SceneFactory::Create(const std::string& name)
{
	if (name == "TITLE")
	{
		return new TitleScene();
	}
	if (name == "PLAY")
	{
		return new PlayScene();
	}
	if (name == "GAMEOVER")
	{
		return new GameOverScene();
	}
	if (name == "STAGESELECT")
	{
		return new StageSelectScene();
	}
	if (name == "SHOP")
	{
		return new ShopScene();
	}
	if (name == "CLEAR")
	{
		return new ClearScene();
	}

	MessageBox(NULL, ("éüÇÃÉVÅ[ÉìÇÕÇ†ÇËÇ‹ÇπÇÒ\n" + name).c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
	assert(false);
	return nullptr;
}