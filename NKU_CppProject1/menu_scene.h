#pragma once

#include "scene.h"
#include "scene_manager.h"
#include <iostream>
#include "player.h"

extern SceneManager scene_manager;
extern IMAGE img_menu_background;	// ���˵�����ͼƬ
extern Player* player;
extern int round_num;

//��Ϸ����ʱ�Ĳ˵�����
class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter()
    {
        round_num = 0;
        delete player;
        mciSendString(_T("stop bgm_game"), NULL, 0, NULL);

    }

    void on_update(int delta)
    {

    }

    void on_draw()
    {
        putimage(0, 0, &img_menu_background);
        outtextxy(540, 540, _T("�������������Ϸ"));

        COLORREF old_color = gettextcolor();

        // ������ʾ�ı�
        const TCHAR* hint_text = _T("(WASD�ƶ���J�������ǵ��л�Ӣ�����뷨)");
        int hint_width = textwidth(hint_text);

        settextcolor(RGB(200, 200, 200)); // ǳ��ɫ��ʾ����
        outtextxy((1280 - hint_width) / 2 ,580, hint_text);
        settextcolor(old_color);
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
        player = new Player();
        
    }
private:

};