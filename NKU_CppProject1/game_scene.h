#pragma once

#include "scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "animation.h"
#include "player.h"
#include <iostream>

extern SceneManager scene_manager;
extern IMAGE img_game_background;	// ��Ϸ��������ͼƬ
extern bool is_debug;

extern Player* player;

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
        timer_game_round.set_timeout([&]() {
            scene_manager.switch_to(SceneManager::SceneType::EventSelection);
            });

    }

    void on_update(int delta)
    {
        std::cout << "GAME IS RUNNING" << std::endl;
        timer_game_round.on_update(delta);
        player->on_update(delta);
    }

    void on_draw()
    {
        putimage(0, 0, &img_game_background);	// ������Ϸ��������ͼƬ
        if (is_debug)
        {
            COLORREF oldColor = gettextcolor();
            settextcolor(RGB(255, 0, 0));  // ��ɫ
            outtextxy(15, 15, _T("�ѿ�������ģʽ������Q�����ر�"));
            settextcolor(oldColor);
        }

        player->on_render();
    }

    void on_input(const ExMessage& msg)
    {
        player->on_input(msg);
        if (msg.message == WM_KEYDOWN && msg.vkcode == 'Q')
        {
            is_debug = !is_debug;
        }
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