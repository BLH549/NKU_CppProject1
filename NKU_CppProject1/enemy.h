#pragma once
#include<iostream>
#include<graphics.h>
#include "vector2.h"

#include "camera.h"
#include "animation.h"

class Player;

extern Player* player;
extern IMAGE img_orc_run_right;
extern IMAGE* img_orc_run_left;
extern bool is_debug;

class Enemy
{
public:
	float run_velocity = 0.10f;//跑动速度

	Vector2 size = { 26,30 };          //人物尺寸
	Vector2 position;                // 人物位置,中心点
	Vector2 velocity;                //速度
	int damage = 20; //伤害值
	int hp = 1; //血量



	Animation animation_run_left;    // 朝向左的奔跑动画
	Animation animation_run_right;   // 朝向右的奔跑动画


	Animation* current_animation = nullptr;

	Vector2 dir_normalized;	//与玩家的方向向量

	bool is_facing_right = true; // 是否面向右侧
	bool is_alive = true;

public:
	Enemy()
	{
		//初始化敌人移动动画
		animation_run_right.add_frame(&img_orc_run_right, 6);
		animation_run_right.set_loop(true);
		animation_run_right.set_interval(100);
		animation_run_right.set_position(this->position);
		flip_image(&img_orc_run_right, img_orc_run_left, 6);
		animation_run_left.add_frame(img_orc_run_left, 6);
		animation_run_left.set_loop(true);
		animation_run_left.set_interval(100);
		animation_run_left.set_position(this->position);

		//初始化敌人位置
		enum class SpawnEdge
		{
			Up = 0,
			Down,
			Left,
			Right
		};


		// 将敌人放置在地图外边界处的随机位置
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge)
		{
		case SpawnEdge::Up:
			position.x = rand() % 1280;
			position.y = -20;
			break;
		case SpawnEdge::Down:
			position.x = rand() % 1280;
			position.y = 740;
			break;
		case SpawnEdge::Left:
			position.x = -20;
			position.y = rand() % 720;
			break;
		case SpawnEdge::Right:
			position.x = 1300;
			position.y = rand() % 720;
			break;
		default:
			break;
		}
	}

	~Enemy() = default;

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

	//核心函数
	void on_run(int delta, const Player* player)
	{

		Vector2 dir = { player->get_position().x - position.x, player->get_position().y - position.y };
		dir_normalized = dir.normalize();

		position.x += (int)(run_velocity * dir_normalized.x * delta);
		position.y += (int)(run_velocity * dir_normalized.y * delta);
	}

	virtual void on_update(int delta, const Player* player)
	{
		//角色移动
		on_run(delta, player);

		//动画更新
		if (dir_normalized.x != 0)
		{
			is_facing_right = dir_normalized.x > 0;
		}
		else
		{
			is_facing_right = is_facing_right;
		}
		current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
		current_animation->on_update(delta);


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

};

