// orc.h
#pragma once
#include "enemy.h"
#include "player.h"

extern IMAGE img_orc_run_right;
extern IMAGE* img_orc_run_left;

class Orc : public Enemy {
private:
    Animation animation_run_left;
    Animation animation_run_right;

public:
	Orc(int hp_increase, int damage_increase)
		: Enemy(hp_increase, damage_increase) 
	{

		//初始化orc移动动画
		animation_run_right.add_frame(&img_orc_run_right, 6);
		animation_run_right.set_loop(true);
		animation_run_right.set_interval(100);
		animation_run_right.set_position(this->position);
		flip_image(&img_orc_run_right, img_orc_run_left, 6);
		animation_run_left.add_frame(img_orc_run_left, 6);
		animation_run_left.set_loop(true);
		animation_run_left.set_interval(100);
		animation_run_left.set_position(this->position);

		current_animation = &animation_run_right;

    }

    

	void on_update(int delta, const Player* player)	override
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


	}
    
};