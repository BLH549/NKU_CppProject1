#pragma once

#include "scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "animation.h"
#include <iostream>

extern SceneManager scene_manager;
extern IMAGE img_game_background;	// ��Ϸ��������ͼƬ

extern Animation test_animation;
extern Animation test_animation_fliped;

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
		test_animation.on_update(delta);
		test_animation_fliped.on_update(delta);
    }

    void on_draw()
    {
		putimage(0, 0, &img_game_background);	// ������Ϸ��������ͼƬ
		test_animation.on_render(1.0f);
		test_animation_fliped.on_render(1.0f);
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