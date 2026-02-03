#pragma once
#include <string>

class SceneBase;

class SceneFactory
{
public:
	SceneBase* CreateFirst();
	SceneBase* Create(const std::string& name);
};
