#pragma once

#include "scene.h"
#include "scene_manager.h"
#include "timer.h"
#include <iostream>

extern SceneManager scene_manager;

//��Ϸ��������
class GameScene : public Scene
{
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        std::cout << "ENTER GAME" << std::endl;

		//��ʼ����Ϸ�غϼ�ʱ��
		timer_game_round.set_wait_time(10000);
        timer_game_round.set_one_shot(false);
        timer_game_round.set_callback([&]() {
            scene_manager.switch_to(SceneManager::SceneType::EventSelection);
            });
    }

    void on_update(int delta)
    {
        std::cout << "GAME IS RUNNING" << std::endl;
		timer_game_round.on_update(delta);
    }

    void on_draw()
    {
        outtextxy(10, 10, _T("GAME"));
    }

    void on_input(const ExMessage& msg)
    {
		//��E�����ز˵�
        if (msg.message == WM_KEYDOWN && msg.vkcode == 'E')
        {
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }

    void on_exit()
    {
        std::cout << "EXIT GAME" << std::endl;
    }
private:
	Timer timer_game_round;     //һ����Ϸ�غϼ�ʱ��

};