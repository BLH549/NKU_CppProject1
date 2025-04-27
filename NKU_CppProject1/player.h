#pragma once

#include<iostream>
#include<graphics.h>
#include "vector2.h"
#include "camera.h"
#include "animation.h"

extern bool is_debug;

extern IMAGE img_player_run_right;
extern IMAGE* img_player_run_left;
extern IMAGE img_player_idle_right;
extern IMAGE* img_player_idle_left;


class Player
{
    //游戏中角色的基础属性
protected:
    const float run_velocity = 0.25f;//跑动速度



    Vector2 size;                      //角色尺寸
    Vector2 position;                // 角色位置,中心点
    Vector2 velocity;                   //角色速度

protected:

    Animation animation_run_left;    // 朝向左的奔跑动画
    Animation animation_run_right;   // 朝向右的奔跑动画

    Animation animation_idle_left;   // 朝向左的静止动画
    Animation animation_idle_right;   // 朝向右的静止动画

    Animation* current_animation = nullptr;

    bool is_left_key_down = false;  // 向左移动按键是否按下
    bool is_right_key_down = false; // 向右移动按键是否按下
    bool is_up_key_down = false;    // 向上移动按键是否按下
    bool is_down_key_down = false;   // 向下移动按键是否按下

    bool is_facing_right = true; // 是否面向右侧
    bool is_moving = false;  // 是否正在移动


public:
    //初始化动画,当前动画
    Player()
    {

        //初始化角色移动动画
        animation_run_right.add_frame(&img_player_run_right, 6);
        animation_run_right.set_loop(true);
        animation_run_right.set_interval(100);
        animation_run_right.set_position(this->position);

        flip_image(&img_player_run_right, img_player_run_left, 6);
        animation_run_left.add_frame(img_player_run_left, 6);
        animation_run_left.set_loop(true);
        animation_run_left.set_interval(100);
        animation_run_left.set_position(this->position);

        //初始化角色静止动画
        animation_idle_right.add_frame(&img_player_idle_right, 4);
        animation_idle_right.set_loop(true);
        animation_idle_right.set_interval(100);
        animation_idle_right.set_position(this->position);

        flip_image(&img_player_idle_right, img_player_idle_left, 4);
        animation_idle_left.add_frame(img_player_idle_left, 4);
        animation_idle_left.set_loop(true);
        animation_idle_left.set_interval(100);
        animation_idle_left.set_position(this->position);


        current_animation = &animation_idle_right;

        //初始化角色位置
        size.x = 46;
        size.y = 58;
        position = { 640,360 };


    }
    ~Player() = default;

    //一些基本的函数
    const Vector2& get_position() const
    {
        return position;
    }
    const Vector2& get_size() const
    {
        return size;
    }


    void set_position(float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    void before_game_reset()
    {
        //重置角色位置
        position.x = 640;
        position.y = 360;
        //重置角色状态
        is_left_key_down = false;
        is_right_key_down = false;
        is_up_key_down = false;
        is_down_key_down = false;
        is_facing_right = true;

        is_moving = false;
    }

    virtual void on_attack() {}

    //重要函数的实现
    //碰撞检测
    void move_and_collide(int delta)
    {

    }

    virtual void on_run(int delta)
    {
        float dir_x = is_right_key_down - is_left_key_down;
        float dir_y = is_down_key_down - is_up_key_down;


        double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
        if (len_dir != 0)
        {
            is_moving = true;
            double normalized_x = dir_x / len_dir;
            double normalized_y = dir_y / len_dir;
            position.x += (int)(run_velocity * normalized_x * delta);
            position.y += (int)(run_velocity * normalized_y * delta);
        }
        else
        {
            is_moving = false;
        }

        //不能移动出屏幕
        if (position.x < 0)	position.x = 0;
        if (position.y < 0)	position.y = 0;
        if (position.x + size.x > 1280)		position.x = 1280 - size.x;
        if (position.y + size.y > 720)	position.y = 720 - size.y;
    }

    //更新状态:朝向，动画，移动，碰撞，计时器
    virtual void on_update(int delta)
    {
        //角色移动
        on_run(delta);

        int direction = is_right_key_down - is_left_key_down;

        if (is_moving)
        {
            if (direction != 0)
            {
				is_facing_right = direction > 0;
			}
            else
            {
                is_facing_right = is_facing_right;
            }

            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
        }
        else
        {

            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        }

        current_animation->on_update(delta);

        move_and_collide(delta);
    }

    virtual void on_render()
    {
        current_animation->set_position(position);
        current_animation->on_render(1.0f);

        if (is_debug)
        {
            setlinecolor(RGB(0, 125, 255));
            rectangle((int)(position.x - size.x / 2), (int)(position.y - size.y / 2), (int)(position.x + size.x / 2), (int)(position.y + size.y / 2));
        }


    }

    virtual void on_input(const ExMessage& msg)
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case VK_UP:
                is_up_key_down = true;
                break;
            case VK_DOWN:
                is_down_key_down = true;
                break;
            case VK_LEFT:
                is_left_key_down = true;
                break;
            case VK_RIGHT:
                is_right_key_down = true;
                break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case VK_UP:
                is_up_key_down = false;
                break;
            case VK_DOWN:
                is_down_key_down = false;
                break;
            case VK_LEFT:
                is_left_key_down = false;
                break;
            case VK_RIGHT:
                is_right_key_down = false;
                break;
            }
            break;
        }
    }

};
