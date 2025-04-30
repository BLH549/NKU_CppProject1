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
#include "entity_id.h"


extern SceneManager scene_manager;
extern IMAGE img_game_background;	// 游戏场景背景图片
extern bool is_debug;
extern int round_num;

extern IMAGE img_player_avatar;

extern Player* player;
extern std::vector<Enemy*> enemy_list;
extern std::vector<Bullet*> bullet_list;

//游戏内主场景
class GameScene : public Scene
{
private:
    Timer timer_game_round;     //一轮游戏回合计时器
    //std::vector<Enemy*> enemy_list;
    Timer timer_spawn_enemy;    //生成敌人计时器
    StatusBar status_bar;  //玩家血条
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        round_num++;

        //初始化游戏回合计时器
        timer_game_round.set_wait_time(50000);
        timer_game_round.set_one_shot(false);
        timer_game_round.set_timeout([&]() {
            scene_manager.switch_to(SceneManager::SceneType::EventSelection);
            });
		timer_game_round.restart();


        timer_spawn_enemy.set_wait_time(500);
        timer_spawn_enemy.set_one_shot(false);
        timer_spawn_enemy.set_timeout([&]() {
            enemy_list.push_back(new Enemy());
            });

        //初始化角色血条
        status_bar.set_avatar(&img_player_avatar);
        status_bar.set_position(10, 10);
    }

    void on_update(int delta)
    {
		// 更新计时器
        timer_game_round.on_update(delta);
        timer_spawn_enemy.on_update(delta);

        // 处理碰撞
        process_collisions();
		
        // 更新玩家
        player->on_update(delta);

        //更新玩家血条
        status_bar.set_hp(player->get_hp());

        //更新子弹
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

        // 更新敌人状态
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



        //游戏结束判定：
		if (player->get_hp() <= 0)
		{
            static TCHAR	 text[128];
            _stprintf_s(text, _T("坚持轮数：%d"), round_num);
            MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);

			//返回菜单
			scene_manager.switch_to(SceneManager::SceneType::Menu );
		}

		
    }

    void on_draw()
    {
        putimage(0, 0, &img_game_background);	// 绘制游戏场景背景图片
        if (is_debug)
        {
            COLORREF oldColor = gettextcolor();
            settextcolor(RGB(255, 0, 0));  // 红色
            outtextxy(15, 15, _T("没关就是开？"));
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
        //按E键返回菜单
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

    // 添加碰撞检测函数
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
		// 敌人与玩家的碰撞检测
        for (Enemy* enemy : enemy_list) {
            if (enemy->check_alive() 
                && check_collision_AABB(player->get_position(), player->get_size(),enemy->get_position(), enemy->get_size()) 
                && !(player->get_invulnerable_State())
               )   
            {
                player->hp_loss(enemy->get_damage());
				player->on_collision();
                std::cout << "Player HP: " << player->get_hp() << std::endl;
            }
        }

		//子弹与玩家，子弹与敌人的碰撞检测
		for (Bullet* bullet : bullet_list)
        {
            if (bullet->get_collide_target() == Entity_ID::Enemy && bullet->get_valid() )
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
				
		    
			
		}

    }


};