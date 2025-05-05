#pragma once

#include "scene.h"
#include <iostream>
#include "scene_manager.h"

extern SceneManager scene_manager;

//事件选择场景
class EventSelectionScene : public Scene
{
public:
    EventSelectionScene() = default;
    ~EventSelectionScene() = default;

    void on_enter()
    {

        
    }

    void on_update(int delta)
    {

    }

    void on_draw()
    {
        outtextxy(610, 350, _T("获得祝福：攻击力提升"));
    }

    void on_input(const ExMessage& msg)
    {
        //按1，2，3键选择事件并切换到游戏场景
        if (msg.message == WM_KEYDOWN)
        {
            if (msg.vkcode >= '1' && msg.vkcode <= '3')
            {
                scene_manager.switch_to(SceneManager::SceneType::Game);
            }
        }
    }

    void on_exit()
    {
        player->before_game_reset();   //重置角色状态
        std::cout << "EXIT eventselection" << std::endl;
    }
private:

};