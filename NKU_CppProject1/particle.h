#pragma once


#include <graphics.h>
#include "vector2.h"


// 定义粒子结构体
struct Particle {
    Vector2 position;     // 当前位置
    int current_radius;   // 当前半径
    COLORREF color;       // 固定颜色
    Vector2 velocity;     // 运动速度
    int decrease_rate;    // 每帧半径衰减量
};

// 粒子类
class ParticleSystem {

public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

   std::vector<Particle> particles;

public:
    // 敌人受伤害效果，发出少量粒子
    void CreateDamageEffect(const Vector2& center) {
        for (int i = 0; i < 5; i++) {
            particles.push_back({
                center,
                5, // 当前半径
                RGB(200 + rand() % 55, 0, 0), // 血红色
                { (float)(rand() % 15 - 7), (float)(rand() % 15 - 7) }, // 速度
                1   // 每帧减少1像素
                });
        }
    }

    // 敌人爆炸效果，发出大量粒子
    void CreateExplosion(const Vector2& center) 
    {
        for (int i = 0; i < 25; i++) {
            particles.push_back({
                center,
                12, // 当前半径
                RGB(200 + rand() % 55, 0, 0), // 血红色
                { (float)(rand() % 15 - 7), (float)(rand() % 15 - 7) }, // 爆炸速度
                1   // 每帧减少1像素
                });
        }
    }

    void UpdateAll() 
    {
        auto it = particles.begin();
        while (it != particles.end()) {
            // 更新位置
            it->position.x += it->velocity.x;
            it->position.y += it->velocity.y;

            // 更新半径
            it->current_radius -= it->decrease_rate;

            // 移除条件
            if (it->current_radius <= 0) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void DrawAll() {
        for (const auto& p : particles) 
        {
            setfillcolor(p.color);
            solidcircle((int)p.position.x, (int)p.position.y, p.current_radius);
        }
    }


};
