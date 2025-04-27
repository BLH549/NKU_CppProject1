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
    //��Ϸ�н�ɫ�Ļ�������
protected:
    const float run_velocity = 0.25f;//�ܶ��ٶ�



    Vector2 size;                      //��ɫ�ߴ�
    Vector2 position;                // ��ɫλ��,���ĵ�
    Vector2 velocity;                   //��ɫ�ٶ�

protected:

    Animation animation_run_left;    // ������ı��ܶ���
    Animation animation_run_right;   // �����ҵı��ܶ���

    Animation animation_idle_left;   // ������ľ�ֹ����
    Animation animation_idle_right;   // �����ҵľ�ֹ����

    Animation* current_animation = nullptr;

    bool is_left_key_down = false;  // �����ƶ������Ƿ���
    bool is_right_key_down = false; // �����ƶ������Ƿ���
    bool is_up_key_down = false;    // �����ƶ������Ƿ���
    bool is_down_key_down = false;   // �����ƶ������Ƿ���

    bool is_facing_right = true; // �Ƿ������Ҳ�
    bool is_moving = false;  // �Ƿ������ƶ�


public:
    //��ʼ������,��ǰ����
    Player()
    {

        //��ʼ����ɫ�ƶ�����
        animation_run_right.add_frame(&img_player_run_right, 6);
        animation_run_right.set_loop(true);
        animation_run_right.set_interval(100);
        animation_run_right.set_position(this->position);

        flip_image(&img_player_run_right, img_player_run_left, 6);
        animation_run_left.add_frame(img_player_run_left, 6);
        animation_run_left.set_loop(true);
        animation_run_left.set_interval(100);
        animation_run_left.set_position(this->position);

        //��ʼ����ɫ��ֹ����
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

        //��ʼ����ɫλ��
        size.x = 46;
        size.y = 58;
        position = { 640,360 };


    }
    ~Player() = default;

    //һЩ�����ĺ���
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
        //���ý�ɫλ��
        position.x = 640;
        position.y = 360;
        //���ý�ɫ״̬
        is_left_key_down = false;
        is_right_key_down = false;
        is_up_key_down = false;
        is_down_key_down = false;
        is_facing_right = true;

        is_moving = false;
    }

    virtual void on_attack() {}

    //��Ҫ������ʵ��
    //��ײ���
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

        //�����ƶ�����Ļ
        if (position.x < 0)	position.x = 0;
        if (position.y < 0)	position.y = 0;
        if (position.x + size.x > 1280)		position.x = 1280 - size.x;
        if (position.y + size.y > 720)	position.y = 720 - size.y;
    }

    //����״̬:���򣬶������ƶ�����ײ����ʱ��
    virtual void on_update(int delta)
    {
        //��ɫ�ƶ�
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
