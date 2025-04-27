#pragma once

#include <iostream>
#include <vector>

#include "scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "animation.h"
#include "player.h"
#include "enemy.h"


extern SceneManager scene_manager;
extern IMAGE img_game_background;	// 游戏场景背景图片
extern bool is_debug;

extern Player* player;

//游戏内主场景
class GameScene : public Scene
{
private:
    Timer timer_game_round;     //一轮游戏回合计时器
    std::vector<Enemy*> enemy_list;
    Timer timer_spawn_enemy;    //生成敌人计时器
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter()
    {
        std::cout << "ENTER GAME" << std::endl;

        //初始化游戏回合计时器
        timer_game_round.set_wait_time(10000);
        timer_game_round.set_one_shot(false);
        timer_game_round.set_timeout([&]() {
            scene_manager.switch_to(SceneManager::SceneType::EventSelection);
            });

        timer_spawn_enemy.set_wait_time(5000);
        timer_spawn_enemy.set_one_shot(false);
        timer_spawn_enemy.set_timeout([&]() {
            enemy_list.push_back(new Enemy());
            });

    }

    void on_update(int delta)
    {
        timer_game_round.on_update(delta);
        timer_spawn_enemy.on_update(delta);
        player->on_update(delta);


        // 清理死亡敌人
        auto enemy = enemy_list.begin();
        while (enemy != enemy_list.end())
        {
            if (!(*enemy)->check_alive())
            {
                delete* enemy;
                enemy = enemy_list.erase(enemy);
            }
            else
            {
                (*enemy)->on_update(delta, player);
                ++enemy;
            }
        }
    }

    void on_draw()
    {
        putimage(0, 0, &img_game_background);	// 绘制游戏场景背景图片
        if (is_debug)
        {
            COLORREF oldColor = gettextcolor();
            settextcolor(RGB(255, 0, 0));  // 红色
            outtextxy(15, 15, _T("已开启调试模式，按‘Q’键关闭"));
            settextcolor(oldColor);
        }

        player->on_render();
        for (Enemy* enemy : enemy_list)
            enemy->on_render();
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
        std::cout << "EXIT GAME" << std::endl;

        for (Enemy* enemy : enemy_list)
            delete enemy;

        enemy_list.clear();
    }


};