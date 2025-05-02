#pragma once

#include "bullet.h"
#include "animation.h"

extern IMAGE shaman_bullet;
extern Atlas atlas_shaman_bullet_break;

class ShamanBullet : public Bullet {
private:
    Animation animation_break; // ��ײ���鶯��

public:
    ShamanBullet(const Vector2& startPos, const Vector2& direction)
    {
        //��ʼ���ӵ����ٶȺ�λ�ã���С
        position = startPos;
		size = { 22, 22 };
        float speed = 0.20f;
        velocity = direction * speed;
        valid = true;

        // ֻ�������������ײ
        collide_with_player = true;
        collide_with_enemy = false;
        collide_with_bullet = false;

        // �����ӵ��ĳߴ�
		size = { 22, 22 };

        // �����ӵ���ͼƬ����ײ���鶯��
        animation_break.add_frame(&atlas_shaman_bullet_break);
        animation_break.set_interval(100);
        animation_break.set_loop(false);
        animation_break.set_on_finished([&]() { can_remove = true; });
    }

    void on_update(int delta)
    {
        //
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        if (!valid)
        {
			animation_break.set_position(position);
            animation_break.on_update(delta);
        }
        if (check_if_exceeds_screen())
            can_remove = true;
    }

    void on_draw() 
    {

        if (valid)
            putimage_alpha( (int)(position.x-size.x/2), (int)(position.y-size.y/2), &shaman_bullet);
        else
            animation_break.on_render(1.0f);

        Bullet::on_draw();
    }

    void on_collide()
    {
        Bullet::on_collide();

        
     }
};
