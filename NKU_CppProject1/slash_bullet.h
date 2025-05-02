#pragma once

#include <iostream>
#include "bullet.h"
#include "animation.h"

extern Atlas atlas_slash_left;
extern Atlas atlas_slash_right;


class SlashBullet : public Bullet {
private:
    Animation animation;
    bool is_facing_right;

public:
    SlashBullet(bool facing_right,float x,float y )
    {
		is_facing_right = facing_right;
		position.x = x;
		position.y = y; 
        velocity = { 0,0 };
		size = { 200, 200 };

        animation.add_frame(facing_right ? &atlas_slash_right : &atlas_slash_left);
        animation.set_interval(8);
        animation.set_loop(false);
		animation.set_position(position);
        animation.set_on_finished([&]() { can_remove = true; });

		// 设置碰撞目标：先默认为敌人，子弹。后续需要修改
        collide_with_enemy = true;
        collide_with_bullet = false;//

    }

    void on_update(int delta) 
    {
        animation.on_update(delta);

        position.x += (int)(velocity.x * delta);
        position.y += (int)(velocity.y * delta);

    }




















    void on_draw() 
    {
        animation.on_render(1.0f);
        Bullet::on_draw();
    }
};