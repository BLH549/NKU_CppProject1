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
    //游戏中角色的基础属性
protected:
    const float run_velocity = 0.25f;//跑动速度


    Vector2 size;                      //角色尺寸
    Vector2 position;                // 角色位置,中心点
    Vector2 velocity;                   //角色速度
    int damage = 90; //角色伤害值
	int hp_max = 300; //角色最大血量
    int hp = 300; //角色血量
	int attack_cd = 500; //角色攻击冷却时间

	bool can_collide_with_bullet = false; //角色是否可以碰撞

protected:
    //动画
    Animation animation_run_left;    // 朝向左的奔跑动画
    Animation animation_run_right;   // 朝向右的奔跑动画

    Animation animation_idle_left;   // 朝向左的静止动画
    Animation animation_idle_right;   // 朝向右的静止动画

    Animation* current_animation = nullptr;

    //计时器
    Timer timer_invulnerable;
    Timer timer_attack_cd;   //攻击计时器

    //角色移动
    bool is_left_key_down = false;  // 向左移动按键是否按下
    bool is_right_key_down = false; // 向右移动按键是否按下
    bool is_up_key_down = false;    // 向上移动按键是否按下
    bool is_down_key_down = false;   // 向下移动按键是否按下
    bool is_facing_right = true; // 是否面向右侧
    bool is_moving = false;  // 是否正在移动
	bool can_attack = true; // 是否可以攻击

	//角色状态
    bool is_alive = true;
	bool is_invulnerable = false; // 是否无敌

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

        //初始化无敌计时器
        timer_invulnerable.set_wait_time(750);
        timer_invulnerable.set_one_shot(true);
        timer_invulnerable.set_timeout([&]()
            {
                is_invulnerable = false;
            });

        //初始化角色攻击计时器

        timer_attack_cd.set_wait_time(attack_cd);
        timer_attack_cd.set_one_shot(false);
        timer_attack_cd.set_timeout([&]()
            {
                can_attack = true;
            });
        

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
        //重置角色位置
        position.x = 640;
        position.y = 360;
        //重置角色移动状态
        is_left_key_down = false;
        is_right_key_down = false;
        is_up_key_down = false;
        is_down_key_down = false;
        is_facing_right = true;

        is_moving = false;

		//重置角色血量
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
		//攻击动画
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

    //重要函数的实现
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

    void on_collision()
	{
		make_invulnerable();
		
	}

    
    
   void on_update(int delta)
    {
        //角色移动
        on_run(delta);

		//动画更新
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

		//计时器更新
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
                // '/' 键
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
