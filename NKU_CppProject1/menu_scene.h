#pragma once

#include "scene.h"
#include "scene_manager.h"
#include <iostream>

extern SceneManager scene_manager;

//游戏进入时的菜单场景
class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter()
    {
        std::cout << "ENTER MENU" << std::endl;
    }

    void on_update(int delta)
    {
        std::cout << "THIS IS MENU" << std::endl;
    }

    void on_draw()
    {
        outtextxy(10, 10, _T("MENU"));
    }

    void on_input(const ExMessage& msg)
    {
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
    }

    void on_exit()
    {
        std::cout << "EXIT MENU" << std::endl;
    }
private:

};