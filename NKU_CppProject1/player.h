#pragma once

#include<iostream>
#include<graphics.h>
#include <vector>

#include "vector2.h"
#include "camera.h"
#include "animation.h"
#include "util.h"
#include "bullet.h"
#include "slash_bullet.h"


extern bool is_debug;

extern IMAGE img_player_run_right;
extern IMAGE* img_player_run_left;
extern IMAGE img_player_idle_right;
extern IMAGE* img_player_idle_left;

extern std::vector<Bullet*> bullet_list;

class Player
{
    //��Ϸ�н�ɫ�Ļ�������
protected:
    const float run_velocity = 0.25f;//�ܶ��ٶ�


    Vector2 size;                      //��ɫ�ߴ�
    Vector2 position;                // ��ɫλ��,���ĵ�
    Vector2 velocity;                   //��ɫ�ٶ�
    int damage = 90; //��ɫ�˺�ֵ
	int hp_max = 300; //��ɫ���Ѫ��
    int hp = 300; //��ɫѪ��
	int attack_cd = 500; //��ɫ������ȴʱ��

	bool can_collide_with_bullet = false; //��ɫ�Ƿ������ײ

protected:
    //����
    Animation animation_run_left;    // ������ı��ܶ���
    Animation animation_run_right;   // �����ҵı��ܶ���

    Animation animation_idle_left;   // ������ľ�ֹ����
    Animation animation_idle_right;   // �����ҵľ�ֹ����

    Animation* current_animation = nullptr;

    //��ʱ��
    Timer timer_invulnerable;
    Timer timer_attack_cd;   //������ʱ��

    //��ɫ�ƶ�
    bool is_left_key_down = false;  // �����ƶ������Ƿ���
    bool is_right_key_down = false; // �����ƶ������Ƿ���
    bool is_up_key_down = false;    // �����ƶ������Ƿ���
    bool is_down_key_down = false;   // �����ƶ������Ƿ���
    bool is_facing_right = true; // �Ƿ������Ҳ�
    bool is_moving = false;  // �Ƿ������ƶ�
	bool can_attack = true; // �Ƿ���Թ���

	//��ɫ״̬
    bool is_alive = true;
	bool is_invulnerable = false; // �Ƿ��޵�

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

        //��ʼ���޵м�ʱ��
        timer_invulnerable.set_wait_time(750);
        timer_invulnerable.set_one_shot(true);
        timer_invulnerable.set_timeout([&]()
            {
                is_invulnerable = false;
            });

        //��ʼ����ɫ������ʱ��

        timer_attack_cd.set_wait_time(attack_cd);
        timer_attack_cd.set_one_shot(false);
        timer_attack_cd.set_timeout([&]()
            {
                can_attack = true;
            });
        

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

    int get_hp()
    {
        return hp;
    }

    void hp_loss(int d)
    {
        hp -= d;
    }


    int get_damage()
    {
        return damage;
    }

    void change_damage(int d)
    {
        damage += d;
    }

    bool check_alive()
    {
        if (hp <= 0)
        {
            is_alive = false;
        }

        return is_alive;
    }

    void before_game_reset()
    {
        //���ý�ɫλ��
        position.x = 640;
        position.y = 360;
        //���ý�ɫ�ƶ�״̬
        is_left_key_down = false;
        is_right_key_down = false;
        is_up_key_down = false;
        is_down_key_down = false;
        is_facing_right = true;

        is_moving = false;

		//���ý�ɫѪ��
		hp = hp_max;

    }

	bool get_invulnerable_State()
	{
		return is_invulnerable;
	}

    void make_invulnerable()
    {
        is_invulnerable = true;
        timer_invulnerable.restart();
       
    }

    void on_attack()
    {
		//��������
		SlashBullet* slash_bullet = new SlashBullet(is_facing_right,position.x,position.y);
        
		timer_attack_cd.restart();

        slash_bullet->set_callback([slash_bullet]() { slash_bullet->set_valid(false); });
		can_attack = false;
		
        slash_bullet->increase_damage(damage);
		slash_bullet->set_collide_with_player(false);
		slash_bullet->set_collide_with_enemy(true);
		slash_bullet->set_collide_with_bullet(can_collide_with_bullet);
        
		bullet_list.push_back(slash_bullet);
		
    }

    //��Ҫ������ʵ��
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

    void on_collision()
	{
		make_invulnerable();
		
	}

    
    
   void on_update(int delta)
    {
        //��ɫ�ƶ�
        on_run(delta);

		//��������
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

		//��ʱ������
        timer_invulnerable.on_update(delta);
		timer_attack_cd.on_update(delta);

    }

    virtual void on_render()
    {
        current_animation->set_position(position);
        current_animation->on_render(1.0f);

        if (is_debug)
        {
            setlinecolor(RGB(0, 125, 255));
            rectangle((int)(position.x - size.x / 2), (int)(position.y - size.y / 2), (int)(position.x + size.x / 2), (int)(position.y + size.y / 2));
			circle((int)position.x, (int)position.y, 100);
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
                // '/' ��
			case VK_OEM_2: 
                if (can_attack)
                {
                    on_attack();
                    can_attack = false;
                    timer_attack_cd.restart();
                }
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
