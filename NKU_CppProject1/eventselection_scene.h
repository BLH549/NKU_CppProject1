#pragma once

#include "scene.h"
#include <iostream>
#include "scene_manager.h"
#include <cstdlib>  
#include "player.h"

extern SceneManager scene_manager;
extern Player* player;

extern IMAGE img_buff_attackcd;
extern IMAGE img_buff_damage;
extern IMAGE img_buff_hp;
extern IMAGE img_buff_slash;
extern IMAGE img_game_background;

//�¼�ѡ�񳡾�
class EventSelectionScene : public Scene
{
public:
    int num = 4;
    // ��ǰѡ�е��¼�����
    int selected_event_index = 0;

    const TCHAR* blessing_name = _T("");

    IMAGE* current_img = nullptr;


public:
    EventSelectionScene() = default;
    ~EventSelectionScene() = default;

    void on_enter()
    {
        selected_event_index = rand() % num;


        switch (selected_event_index) {
        case 0: // ����������
            player->change_damage(20);
            current_img = &img_buff_damage;
            blessing_name = _T("����������");
            break;
        case 1: // �����ٶ�����
            player->attack_cd -= 100;
            if (player->attack_cd < 20) player->attack_cd = 20;
            current_img = &img_buff_attackcd;
            blessing_name = _T("ף��:������ȴ����");
            break;
        case 2: // �������ֵ����
            player->hp_max += 50;
            current_img = &img_buff_hp;
            blessing_name = _T("ף��:�������ֵ����");
            break;
        case 3: //�����¼���ն���ӵ�
            player->can_collide_with_bullet = true;
            current_img = &img_buff_slash;
            blessing_name = _T("ϡ��ף��:�ӵ����Ա�����");
            num=num - 1;    //ֻ�ܻ��һ��
        }
    }

    void on_update(int delta)
    {

    }



    void on_draw()
    {
        putimage_alpha(0, 0, &img_game_background);
        
        putimage_alpha((1280 - current_img->getwidth()) / 2, 200, current_img);

        // ����ף���ı�
        TCHAR text[128];
        _stprintf_s(text, blessing_name);

        // �����ı�λ��
        int text_width = textwidth(text);
        int text_x = (1280 - text_width) / 2;
        int text_y = 200 + current_img->getheight() + 20; // ͼ���·�20����

        // �������ɫ����������ɫ
        COLORREF old_color = gettextcolor();
        settextcolor(RGB(255, 255, 255)); // ��ɫ����

        // �����ı�
        outtextxy(text_x, text_y, text);

        // �ָ�����ɫ
        settextcolor(old_color);

        // ������ʾ�ı�
        const TCHAR* hint_text = _T("��enter������");
        int hint_width = textwidth(hint_text);
        int hint_x = (1280 - hint_width) / 2;
        int hint_y = text_y + 40; // ���ı��·�40����

        settextcolor(RGB(200, 200, 200)); // ǳ��ɫ��ʾ����
        outtextxy(hint_x, hint_y, hint_text);
        settextcolor(old_color);
        
    }

    void on_input(const ExMessage& msg)
    {
        //��1��2��3��ѡ���¼����л�����Ϸ����
        if (msg.message == WM_KEYDOWN)
        {
            if (msg.vkcode == VK_RETURN)
            {
                scene_manager.switch_to(SceneManager::SceneType::Game);
            }
        }
    }

    void on_exit()
    {
        player->before_game_reset();   //���ý�ɫ״̬
        
    }



};