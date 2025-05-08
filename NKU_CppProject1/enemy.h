#pragma once

#include "vector2.h"
#include "animation.h"
#include "player.h"
#include "particle.h"

extern bool is_debug;
extern Player* player;
extern ParticleSystem* particle_system;

class Enemy {
protected:
    float run_velocity = 0.10f;
    Vector2 size = { 26,30 };
    Vector2 position;
    Vector2 velocity;
    int damage = 0;
    int hp = 0;
	int base_hp = 1;
	int base_damage = 1;

    Animation* current_animation = nullptr;
    Vector2 dir_player_normalized;
    bool is_facing_right = true;

public:
    
    Enemy()
    {
		
        // 位置初始化逻辑
        //初始化敌人位置
        enum class SpawnEdge
        {
            Up = 0,
            Down,
            Left,
            Right
        };

		// 将敌人放置在地图外边界处的随机位置
        SpawnEdge edge = static_cast<SpawnEdge>(rand() % 4);
        switch (edge) {
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
        }
    }

    virtual ~Enemy() = default;

    //重要函数
    virtual void on_attack() {}  

    virtual void on_update(int delta, const Player* player) = 0;

    virtual void on_render()
    {
        current_animation->set_position(position);
        current_animation->on_render(1.0f);
        
        if (is_debug)
        {
        	setlinecolor(RGB(0, 125, 255));
        	rectangle((int)(position.x - size.x / 2), (int)(position.y - size.y / 2), (int)(position.x + size.x / 2), (int)(position.y + size.y / 2));
        	circle((int)position.x, (int)position.y, 5);
        }
        
        
     }

    void on_run(int delta, const Player* player)
    {
        
        Vector2 dir = { player->get_position().x - position.x, player->get_position().y - position.y };
        dir_player_normalized = dir.normalize();
        
        position.x += (int)(run_velocity * dir_player_normalized.x * delta);
        position.y += (int)(run_velocity * dir_player_normalized.y * delta);

    }


	//一些基本的函数
    Vector2& get_position() { return position; }

	void set_position(float x, float y) { position = { x, y }; }

    Vector2& get_size()  { return size; }

    bool check_alive() { return hp > 0; }

    void hp_loss(int d) 
    { 
        hp -= d; 
        particle_system->CreateDamageEffect(position);

		if (hp <= 0)
		{
			particle_system->CreateExplosion(position);
            mciSendString(_T("play hit from 0"), NULL, 0, NULL);
		}

        
    
    }

	int get_damage() { return damage; }

	void change_damage(int d) { damage += d; }

	void set_run_velocity(float velocity) { run_velocity = velocity; }
	
};

