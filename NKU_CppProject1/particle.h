#pragma once


#include <graphics.h>
#include "vector2.h"


// �������ӽṹ��
struct Particle {
    Vector2 position;     // ��ǰλ��
    int current_radius;   // ��ǰ�뾶
    COLORREF color;       // �̶���ɫ
    Vector2 velocity;     // �˶��ٶ�
    int decrease_rate;    // ÿ֡�뾶˥����
};

// ������
class ParticleSystem {

public:
	ParticleSystem() = default;
	~ParticleSystem() = default;

   std::vector<Particle> particles;

public:
    // �������˺�Ч����������������
    void CreateDamageEffect(const Vector2& center) {
        for (int i = 0; i < 5; i++) {
            particles.push_back({
                center,
                5, // ��ǰ�뾶
                RGB(200 + rand() % 55, 0, 0), // Ѫ��ɫ
                { (float)(rand() % 15 - 7), (float)(rand() % 15 - 7) }, // �ٶ�
                1   // ÿ֡����1����
                });
        }
    }

    // ���˱�ըЧ����������������
    void CreateExplosion(const Vector2& center) 
    {
        for (int i = 0; i < 25; i++) {
            particles.push_back({
                center,
                12, // ��ǰ�뾶
                RGB(200 + rand() % 55, 0, 0), // Ѫ��ɫ
                { (float)(rand() % 15 - 7), (float)(rand() % 15 - 7) }, // ��ը�ٶ�
                1   // ÿ֡����1����
                });
        }
    }

    void UpdateAll() 
    {
        auto it = particles.begin();
        while (it != particles.end()) {
            // ����λ��
            it->position.x += it->velocity.x;
            it->position.y += it->velocity.y;

            // ���°뾶
            it->current_radius -= it->decrease_rate;

            // �Ƴ�����
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
