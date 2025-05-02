#pragma once

#include "enemy.h"
#include "shaman_bullet.h"
#include "bullet.h"
#include "timer.h"

#include <cstdlib>  

extern std::vector<Bullet*> bullet_list;
extern Player* player;

extern bool is_debug;
extern IMAGE img_shaman_run_right;
extern IMAGE* img_shaman_run_left;

class Shaman : public Enemy {
private:
    
    Animation animation_run_left;
    Animation animation_run_right;
    
    Timer timer_attack_cd;



public:
    
    Shaman(int hp_increase, int damage_increase)
        : Enemy(hp_increase, damage_increase)
    {
        //初始化shaman的基础属性:
		size = { 38,38 };
        
        animation_run_right.add_frame(&img_shaman_run_right, 6);
        animation_run_right.set_loop(true);
        animation_run_right.set_interval(100);
        animation_run_right.set_position(this->position);
        flip_image(&img_shaman_run_right, img_shaman_run_left, 6);
        animation_run_left.add_frame(img_shaman_run_left, 6);
        animation_run_left.set_loop(true);
        animation_run_left.set_interval(100);
        animation_run_left.set_position(this->position);

        
        current_animation = &animation_run_right;

        // 
        timer_attack_cd.set_wait_time(3000);
        timer_attack_cd.set_one_shot(false);
        timer_attack_cd.set_timeout([this]() {
			spawn_bullet();
            });
    }

	void spawn_bullet() {
        Vector2 dir = { player->get_position().x - position.x, player->get_position().y - position.y };
        dir_player_normalized = dir.normalize();
		
		ShamanBullet* bullet = new ShamanBullet(this->position, dir_player_normalized);
		bullet->set_collide_with_player(true);
		bullet->set_collide_with_enemy(false);
		bullet->set_collide_with_bullet(false);
		
        bullet->increase_damage(5);
        bullet->set_callback([bullet]() { bullet->set_valid(false); });
		bullet_list.push_back(bullet);
	}


    void on_update(int delta, const Player* player) 
    {
        

        //角色移动
        on_run(delta, player);

        //动画更新
        if (dir_player_normalized.x != 0)
        {
            is_facing_right = dir_player_normalized.x > 0;
        }
        else
        {
            is_facing_right = is_facing_right;
        }
        current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
        current_animation->on_update(delta);

		// 更新计时器
        timer_attack_cd.on_update(delta);
    }

   
};
