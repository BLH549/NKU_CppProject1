#pragma once

#include <iostream>
#include <vector>
#include <graphics.h>

#include "scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "animation.h"
#include "status_bar.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "slash_bullet.h"



extern SceneManager scene_manager;
extern IMAGE img_game_background;	// ��Ϸ��������ͼƬ
extern bool is_debug;
extern int round_num;

extern IMAGE img_player_avatar;

extern Player* player;
extern std::vector<Enemy*> enemy_list;
extern std::vector<Bullet*> bullet_list;

//��Ϸ��������
class GameScene : public Scene
{
private:
    Timer timer_game_round;     //һ����Ϸ�غϼ�ʱ��
    Timer timer_spawn_orc;    //����С�ּ�ʱ��
    StatusBar status_bar;  //���״̬��
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        round_num++;

        //��ʼ����Ϸ�غϼ�ʱ��
        timer_game_round.set_wait_time(10000);
        timer_game_round.set_one_shot(false);
        timer_game_round.set_timeout([&]() {
            scene_manager.switch_to(SceneManager::SceneType::EventSelection);
            });
		timer_game_round.restart();


        timer_spawn_orc.set_wait_time(500);
        timer_spawn_orc.set_one_shot(false);
        timer_spawn_orc.set_timeout([&]() {
            enemy_list.push_back(new Enemy());
            });

        //��ʼ����ɫ״̬
        status_bar.set_avatar(&img_player_avatar);
        status_bar.set_position(10, 10);
    }

    void on_update(int delta)
    {
		// ���¼�ʱ��
        timer_game_round.on_update(delta);
        timer_spawn_orc.on_update(delta);

        // ������ײ
        process_collisions();
		
        // �������
        player->on_update(delta);

        //�������Ѫ��
        status_bar.set_hp(player->get_hp());

        //�����ӵ�
        auto bullet = bullet_list.begin();
        while (bullet != bullet_list.end())
        {
            if ((*bullet)->check_can_remove())
            {
                delete* bullet;
                bullet = bullet_list.erase(bullet);
            }
            else
            {
                (*bullet)->on_update(delta);
                ++bullet;
            }
        }

        // ���µ���״̬
        auto enemy = enemy_list.begin();
        while (enemy != enemy_list.end())
        {
            if (!(*enemy)->check_alive())
            {
                delete* enemy;
                enemy=enemy_list.erase(enemy);
            }
            else
            {
                (*enemy)->on_update(delta, player);
                ++enemy;
            }
        }



        //��Ϸ�����ж���
		if (player->get_hp() <= 0)
		{
            static TCHAR	 text[128];
            _stprintf_s(text, _T("���������%d"), round_num);
            MessageBox(GetHWnd(), text, _T("��Ϸ����"), MB_OK);

			//���ز˵�
			scene_manager.switch_to(SceneManager::SceneType::Menu );
		}

		
    }

    void on_draw()
    {
        putimage(0, 0, &img_game_background);	// ������Ϸ��������ͼƬ
        if (is_debug)
        {
            COLORREF oldColor = gettextcolor();
            settextcolor(RGB(255, 0, 0));  // ��ɫ
            outtextxy(15, 15, _T("û�ؾ��ǿ���"));
            settextcolor(oldColor);
        }

        player->on_render();
        for (Enemy* enemy : enemy_list)
            enemy->on_render();
        for (Bullet* bullet : bullet_list)
            bullet->on_draw();

        status_bar.on_draw();
    }

    void on_input(const ExMessage& msg)
    {
        player->on_input(msg);
        if (msg.message == WM_KEYDOWN && msg.vkcode == 'Q')
        {
            is_debug = !is_debug;
        }
        //��E�����ز˵�
        if (msg.message == WM_KEYDOWN && msg.vkcode == 'E')
        {
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }


    }

    void on_exit()
    {
        for (Enemy* enemy : enemy_list)
            delete enemy;

        enemy_list.clear();
    }

    // �����ײ��⺯��
    bool check_collision_point(const Vector2& rect_pos, const Vector2& rect_size, const Vector2& point)
    {
        float left = rect_pos.x - rect_size.x / 2;
        float right = rect_pos.x + rect_size.x / 2;
        float top = rect_pos.y - rect_size.y / 2;
        float bottom = rect_pos.y + rect_size.y / 2;

        return (point.x >= left) && (point.x <= right) &&
            (point.y >= top) && (point.y <= bottom);
    }
    
	//��������ε���ײ���
    bool check_collision_AABB(const Vector2& pos1, const Vector2& size1,const Vector2& pos2, const Vector2& size2) 
    {
        float left1 = pos1.x - size1.x / 2;
        float right1 = pos1.x + size1.x / 2;
        float top1 = pos1.y - size1.y / 2;
        float bottom1 = pos1.y + size1.y / 2;

        float left2 = pos2.x - size2.x / 2;
        float right2 = pos2.x + size2.x / 2;
        float top2 = pos2.y - size2.y / 2;
        float bottom2 = pos2.y + size2.y / 2;

        return (right1 > left2) && (left1 < right2) &&
            (bottom1 > top2) && (top1 < bottom2);
    }


    void process_collisions() 
    {
		// ��������ҵ���ײ���
        for (Enemy* enemy : enemy_list) {
            if (enemy->check_alive() 
                && check_collision_point(player->get_position(), player->get_size(),enemy->get_position()) 
                && !(player->get_invulnerable_State())
               )   
            {
                player->hp_loss(enemy->get_damage());
				player->on_collision();
                
            }
        }

        // �ӵ�����ˡ��ӵ����ӵ�����ײ���
        for (Bullet* bullet : bullet_list)
        {
            if (bullet->get_valid())
            {
                // ����ӵ������
                if (bullet->collide_with_enemy)
                {
                    for (Enemy* enemy : enemy_list)
                    {
                        if (enemy->check_alive()
                            && check_collision_AABB(bullet->get_position(), bullet->get_size(), enemy->get_position(), enemy->get_size())
                            )
                        {
                            enemy->hp_loss(bullet->get_damage());
                        }
                    }
                }

                // ����ӵ��������ӵ�
                if (bullet->collide_with_bullet)
                {
                    for (Bullet* other_bullet : bullet_list)
                    {
                        // ����������
                        if (other_bullet != bullet && other_bullet->get_valid())
                        {
                            if (check_collision_AABB(bullet->get_position(), bullet->get_size(),
                                other_bullet->get_position(), other_bullet->get_size()))
                            {
								other_bullet->set_valid(false);
								other_bullet->on_collide();
                            }
                        }
                    }
                }

				// ����ӵ������
                if (bullet->collide_with_player)
                {
                    if (check_collision_point(player->get_position(), player->get_size(),bullet->get_position()))
                    {
                        player->hp_loss(bullet->get_damage());
                    }
                }


            }
        }

    }


};