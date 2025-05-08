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

//事件选择场景
class EventSelectionScene : public Scene
{
public:
    int num = 4;
    // 当前选中的事件索引
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
        case 0: // 攻击力提升
            player->change_damage(20);
            current_img = &img_buff_damage;
            blessing_name = _T("攻击力提升");
            break;
        case 1: // 攻击速度提升
            player->attack_cd -= 100;
            if (player->attack_cd < 20) player->attack_cd = 20;
            current_img = &img_buff_attackcd;
            blessing_name = _T("祝福:攻击冷却缩短");
            break;
        case 2: // 最大生命值提升
            player->hp_max += 50;
            current_img = &img_buff_hp;
            blessing_name = _T("祝福:最大生命值提升");
            break;
        case 3: //特殊事件，斩碎子弹
            player->can_collide_with_bullet = true;
            current_img = &img_buff_slash;
            blessing_name = _T("稀有祝福:子弹可以被击碎");
            num=num - 1;    //只能获得一次
        }
    }

    void on_update(int delta)
    {

    }



    void on_draw()
    {
        putimage_alpha(0, 0, &img_game_background);
        
        putimage_alpha((1280 - current_img->getwidth()) / 2, 200, current_img);

        // 绘制祝福文本
        TCHAR text[128];
        _stprintf_s(text, blessing_name);

        // 计算文本位置
        int text_width = textwidth(text);
        int text_x = (1280 - text_width) / 2;
        int text_y = 200 + current_img->getheight() + 20; // 图像下方20像素

        // 保存旧颜色并设置新颜色
        COLORREF old_color = gettextcolor();
        settextcolor(RGB(255, 255, 255)); // 白色文字

        // 绘制文本
        outtextxy(text_x, text_y, text);

        // 恢复旧颜色
        settextcolor(old_color);

        // 绘制提示文本
        const TCHAR* hint_text = _T("按enter键继续");
        int hint_width = textwidth(hint_text);
        int hint_x = (1280 - hint_width) / 2;
        int hint_y = text_y + 40; // 主文本下方40像素

        settextcolor(RGB(200, 200, 200)); // 浅灰色提示文字
        outtextxy(hint_x, hint_y, hint_text);
        settextcolor(old_color);
        
    }

    void on_input(const ExMessage& msg)
    {
        //按1，2，3键选择事件并切换到游戏场景
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
        player->before_game_reset();   //重置角色状态
        
    }



};