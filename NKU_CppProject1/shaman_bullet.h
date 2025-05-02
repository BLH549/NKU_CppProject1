#pragma once

#include "bullet.h"
#include "animation.h"

extern IMAGE shaman_bullet;
extern Atlas atlas_shaman_bullet_break;

class ShamanBullet : public Bullet {
private:
    Animation animation_break; // 碰撞破碎动画

public:
    ShamanBullet(const Vector2& startPos, const Vector2& direction)
    {
        //初始化子弹的速度和位置，大小
        position = startPos;
		size = { 22, 22 };
        float speed = 0.20f;
        velocity = direction * speed;
        valid = true;

        // 只能主动与玩家碰撞
        collide_with_player = true;
        collide_with_enemy = false;
        collide_with_bullet = false;

        // 设置子弹的尺寸
		size = { 22, 22 };

        // 设置子弹的图片与碰撞破碎动画
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
