#pragma once

#include "vector2.h"
#include "camera.h"

#include <graphics.h>
#include <functional>


extern bool is_debug;

class Bullet
{
public:
    Vector2 size;            // 子弹尺寸,默认矩形
    Vector2 position;        // 子弹位置
    Vector2 velocity;        // 子弹速度
    int damage = 0;         // 子弹伤害
    bool valid = true;       // 子弹是否有效，能否碰撞
    bool can_remove = false; // 子弹是否可以被移除
    std::function<void()> callback; // 子弹碰撞回调函数
    

	//设置子弹碰撞目标
    bool collide_with_player = false;
    bool collide_with_enemy = false;
    bool collide_with_bullet = false;

public:
    Bullet() = default;
    ~Bullet() = default;


    //基本函数
    void increase_damage(int val)
    {
        damage += val;
    }

    int get_damage()
    {
        return damage;
    }

    void set_position(float x, float y)
    {
        position.x = x, position.y = y;
    }

    const Vector2& get_position() const
    {
        return position;
    }

    Vector2& get_size()
    {
        return size;
    }

    void set_size(float w,float h)
    {
        size.x = w;
        size.y = h;
    }

    void set_velocity(float x, float y)
    {
        velocity.x = x, velocity.y = y;
    }

    // 设置碰撞目标的方法
    void set_collide_with_player(bool enable) { collide_with_player = enable; }
    void set_collide_with_enemy(bool enable) { collide_with_enemy = enable; }
    void set_collide_with_bullet(bool enable) { collide_with_bullet = enable; }

    void set_callback(std::function<void()> callback)
    {
        this->callback = callback;
    }

    void set_valid(bool flag)
    {
        valid = flag;
    }

    bool get_valid() const
    {
        return valid;
    }

    bool check_can_remove() const
    {
        return can_remove;
    }

    virtual void on_collide()
    {
        if (callback)
            callback();
    }

    bool check_if_exceeds_screen()
    {
        return (position.x + size.x <= 0 || position.x >= getwidth()
            || position.y + size.y <= 0 || position.y >= getheight());
    }

    virtual void on_update(int delta) {}
    virtual void on_draw() 
    {
        if (is_debug)
        {
            setfillcolor(RGB(255, 255, 255));
            setlinecolor(RGB(255, 255, 255));
            rectangle((int)(position.x- size.x / 2), (int)(position.y- size.y / 2),
                (int)(position.x + size.x/2), (int)(position.y + size.y/2));
            //solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
        }
    }

    


};
