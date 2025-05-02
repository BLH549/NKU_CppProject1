#pragma once

#include<graphics.h>
#include "util.h"

class StatusBar
{
public:
    StatusBar() = default;
    ~StatusBar() = default;

    void set_avatar(IMAGE* img)
    {
        img_avatar = img;
    }

    void set_position(int x, int y)
    {
        position.x = x, position.y = y;
    }

    void set_hp(int val)
    {
        hp = val;
    }

    void on_draw()
    {
        putimage_alpha(position.x, position.y, img_avatar);

        float hp_bar_width = width * max(0, hp) / 100.0f;
        
        setfillcolor(RGB(197, 61, 67));
        solidroundrect(position.x + 60, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8);
        
    }

private:
    const int width = 275;
private:
    int hp = 0;      // 需要显示的生命值
    POINT position = { 0 };  // 在窗口中显示的位置
    IMAGE* img_avatar = nullptr;  // 角色头像图片
};