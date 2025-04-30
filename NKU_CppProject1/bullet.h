#pragma once

#include "vector2.h"
#include "entity_id.h"
#include "camera.h"

#include <graphics.h>
#include <functional>


extern bool is_debug;

class Bullet
{
public:
    Vector2 size;            // �ӵ��ߴ�,Ĭ�Ͼ���
    Vector2 position;        // �ӵ�λ��
    Vector2 velocity;        // �ӵ��ٶ�
    int damage = 0;         // �ӵ��˺�
    bool valid = true;       // �ӵ��Ƿ���Ч
    bool can_remove = false; // �ӵ��Ƿ���Ա��Ƴ�
    std::function<void()> callback; // �ӵ���ײ�ص�����
    Entity_ID target_id = Entity_ID::Enemy;  //�ӵ���ײĿ��

public:
    Bullet() = default;
    ~Bullet() = default;


    //��������
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

    void set_collide_target(Entity_ID target)
    {
        target_id = target;
    }

    Entity_ID get_collide_target() 
    {
        return target_id;
    }

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
