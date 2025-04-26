#pragma once

#include "scene.h"
#include "scene_manager.h"
#include <iostream>

extern SceneManager scene_manager;
extern IMAGE img_menu_background;	// 主菜单背景图片

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
        putimage(0, 0, &img_menu_background);
        outtextxy(540, 540, _T("按任意键进入游戏"));
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