#pragma once

#include "scene.h"
#include <iostream>
#include "scene_manager.h"

extern SceneManager scene_manager;

//�¼�ѡ�񳡾�
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
        outtextxy(610, 350, _T("���ף��������������"));
    }

    void on_input(const ExMessage& msg)
    {
        //��1��2��3��ѡ���¼����л�����Ϸ����
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
        player->before_game_reset();   //���ý�ɫ״̬
        std::cout << "EXIT eventselection" << std::endl;
    }
private:

};