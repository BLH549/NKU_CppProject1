#pragma once

#include "scene.h"
#include "scene_manager.h"
#include <iostream>
#include "player.h"

extern SceneManager scene_manager;
extern IMAGE img_menu_background;	// 主菜单背景图片
extern Player* player;
extern int round_num;

//游戏进入时的菜单场景
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
        outtextxy(540, 540, _T("按任意键进入游戏"));

        COLORREF old_color = gettextcolor();

        // 绘制提示文本
        const TCHAR* hint_text = _T("(WASD移动，J攻击，记得切换英文输入法)");
        int hint_width = textwidth(hint_text);

        settextcolor(RGB(200, 200, 200)); // 浅灰色提示文字
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