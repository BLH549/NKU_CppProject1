# 南开高级语言程序设计2-2大作业项目报告
## 1.  简介
### 1.1 项目名称
**CHT历险记**
### 1.2 开发软件
Visual Studio 2022 + EasyX
### 1.3 项目简介
一个2D的类肉鸽小游戏，玩家操作一名骑士面对大量敌人的进攻，尽可能地存活并击杀尽量多的敌人。每轮结束后，玩家会获得祝福，但同时敌人也会随着轮数增加而变强。

## 2. 项目设计思路
### 2.1 场景管理
在本项目中，需要频繁的切换不同的场景，所以借助面向对象的特性，采用场景管理器来同一管理所有的场景。
#### 2.1.1 场景类的实现
定义抽象的Scene基类，其核心成员方法为
```cpp
virtual void on_enter() {}    
virtual void on_update(int delta) {}
virtual void on_draw() {}
virtual void on_input(const ExMessage& msg) {}
virtual void on_exit() {}
```
GameScene、MenuScene、EventSelection 作为具体子类，分别重写基类方法，实现游戏场景、主菜单场景、事件选择场景的独有逻辑。
例如，GameScene 的 on_update 方法处理游戏逻辑更新，on_draw 方法负责渲染游戏画面。


下面是MenuScene的on_draw
```cpp
void on_draw()
{
    putimage(0, 0, &img_menu_background);
    outtextxy(540, 540, _T("按任意键进入游戏"));

    COLORREF old_color = gettextcolor();

    // 绘制提示文本
    const TCHAR* hint_text = _T("(WASD移动，J攻击，记得切换英文输入法)");
    int hint_width = textwidth(hint_text);

    settextcolor(RGB(200, 200, 200)); // 浅灰色
    outtextxy((1280 - hint_width) / 2 ,580, hint_text);
    settextcolor(old_color);
}
```

#### 2.1.2 场景的统一处理

SceneManager 作为场景管理器，持有 Scene 基类指针 current_scene。其部分成员方法如下：
```cpp
Scene* current_scene = nullptr;

    void on_update(int delta) {}
    {
        current_scene->on_update(delta);
    }

    void on_draw()
    {
        current_scene->on_draw();
    }

    void on_input(const ExMessage& msg)
    {
        current_scene->on_input(msg);
    }
```
在游戏主循环中，通过调用 current_scene 的 on_update、on_draw、on_input 方法，实现场景的统一更新、绘制与输入处理，确保场景切换的流畅性与逻辑一致性。例如：在主循环中,游戏的更新逻辑如下
```cpp
//更新
static DWORD last_tick_time = GetTickCount();
DWORD current_tick_time = GetTickCount();
DWORD delta_tick = current_tick_time - last_tick_time;
scene_manager.on_update(delta_tick);
```

#### 2.1.3 场景之间跳转逻辑的实现

通过SceneManager中的switch_to方法实现场景的切换。
```cpp
void switch_to(SceneType type)
{
    current_scene->on_exit();
    switch (type)
    {
    case SceneType::Menu:
		current_scene = menu_scene;
        break;
    case SceneType::Game:
		current_scene = game_scene;
        break;
	case SceneType::EventSelection:
		current_scene = eventselection_scene;
		break;

    default:
        break;
    }
    current_scene->on_enter();
}
```
在具体的场景子类中调用switch_to,实现场景的切换。例如game_scene中每轮时间结束时，调用switch_to，切换至EventSelection场景。
```cpp
timer_game_round.set_timeout([&]() {
    scene_manager.switch_to(SceneManager::SceneType::EventSelection);
    });
```

### 2.2 动画类的实现

定义 Animation 类管理游戏对象动画，通过 std::vector 存储帧序列，使用 Timer 控制帧播放间隔。每隔一段时间切换下一帧，逻辑如下：
```cpp
        timer.set_one_shot(false);
        timer.set_timeout(
            [&]()
            {
                idx_frame++;
                if (idx_frame >= frame_list.size())
                {
                    idx_frame = is_loop ? 0 : frame_list.size() - 1;
                    if (!is_loop && on_finished)
                    {
                        on_finished();
                    }
                }
            }

```
角色、敌人和子弹等对象持有 Animation 实例，在各自的on_update 方法中调用动画更新逻辑，实现动画播放效果。
### 2.3 玩家的实现
玩家类 Player 实现了玩家的移动,攻击，渲染等逻辑。受限于篇幅，下面以Player类中的on_update,on_input,on_render方法简要介绍一下。

在 on_update 方法中处理玩家的移动和动画更新：
```cpp
void on_update(int delta)
 {
     //角色移动
     on_run(delta);
		//动画更新
     int direction = is_right_key_down - is_left_key_down;
     if (is_moving)
     {
         if (direction != 0)
         {
             is_facing_right = direction > 0;
         }
         else
         {
             is_facing_right = is_facing_right;
         }

         current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
     }
     else
     {

         current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
     }
     current_animation->on_update(delta);
		//计时器更新
     timer_invulnerable.on_update(delta);
		timer_attack_cd.on_update(delta);

 }

```
在 on_input 方法中处理玩家的输入：

```cpp
void on_input(const ExMessage& msg)
{
    switch (msg.message)
    {
    case WM_KEYDOWN:
        switch (msg.vkcode)
        {
        case 'W': // 向上移动
            is_up_key_down = true;
            break;
        case 'S': // 向下移动
            is_down_key_down = true;
            break;
        case 'A': // 向左移动
            is_left_key_down = true;
            break;
        case 'D': // 向右移动
            is_right_key_down = true;
            break;
        case 'J': // 攻击
            if (can_attack)
            {
                on_attack();
                can_attack = false;
                timer_attack_cd.restart();
            }
            break;
        }
        break;

    case WM_KEYUP:
        switch (msg.vkcode)
        {
        case 'W': // 停止向上移动
            is_up_key_down = false;
            break;
        case 'S': // 停止向下移动
            is_down_key_down = false;
            break;
        case 'A': // 停止向左移动
            is_left_key_down = false;
            break;
        case 'D': // 停止向右移动
            is_right_key_down = false;
            break;
        }
        break;
    }
}
```
在on_render()方法上实现角色的绘制
```cpp
void on_render()
{
    current_animation->set_position(position);
    current_animation->on_render(1.0f);

    if (is_debug)
    {
        setlinecolor(RGB(0, 125, 255));
        rectangle((int)(position.x - size.x / 2), (int)(position.y - size.y / 2), (int)(position.x + size.x / 2), (int)(position.y + size.y / 2));
		circle((int)position.x, (int)position.y, 100);
    }


}
```



### 2.4 敌人的实现
敌人分为 Orc 和 Shaman 等类型，它们都继承自 基类Enemy 类。由统一的enemy_list进行管理与更新。

在enemy类中的构造函数中实现敌人随机生成在地图边缘：
```cpp
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
```
orc只能通过与玩家碰撞才能对玩家造成伤害，其on_update方法如下：
```cpp
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
```
shaman不仅能够通过与玩家碰撞造成伤害，还能通过发射子弹造成伤害。但其on_update与orc类似，不再赘述。

### 2.5 子弹类的实现

子弹类 Bullet 及其子类实现了子弹的移动和碰撞检测,由统一的bullet_list进行管理与更新。以ShamanBullet 为例，介绍Bullet类。

ShamanBullet是由shaman发射的子弹。由shaman中的spawn_bullet方法生成
```cpp
void spawn_bullet() {
    Vector2 dir = { player->get_position().x - position.x, player->get_position().y - position.y };
    dir_player_normalized = dir.normalize();
	
	ShamanBullet* bullet = new ShamanBullet(this->position, dir_player_normalized);
	bullet->set_collide_with_player(true);
	bullet->set_collide_with_enemy(false);
	bullet->set_collide_with_bullet(false);
	
    bullet->increase_damage((int)(1.5*damage));
    bullet->set_callback([bullet]() { bullet->set_valid(false); });
	bullet_list.push_back(bullet);
}
```

ShamanBullet的构造函数如下：
```cpp
ShamanBullet(const Vector2& startPos, const Vector2& direction)
{
    //初始化子弹的速度和位置，大小
    position = startPos;
	size = { 22, 22 };
    float speed = 0.20f;
    velocity = direction * speed;
    valid = true;

    // 只能与玩家碰撞
    collide_with_player = true;
    collide_with_enemy = false;
    collide_with_bullet = false;

    // 设置子弹的尺寸
	size = { 22, 22 };

    // 设置子弹的图片与碰撞破碎动画
    animation_break.add_frame(&atlas_shaman_bullet_break);
    animation_break.set_interval(100);
    animation_break.set_loop(false);
    animation_break.set_on_finished([&]() { can_remove = true; });
}
```

ShamanBullet的 on_update 方法如下：
```cpp
void on_update(int delta)
{
    //
    position.x += velocity.x * delta;
    position.y += velocity.y * delta;

    if (!valid)
    {
		animation_break.set_position(position);
        animation_break.on_update(delta);
    }
    if (check_if_exceeds_screen())
        can_remove = true;
}

```

### 2.6 碰撞的实现
碰撞同一由game_scene中的process_collosion处理，主要采用AABB这一基本算法
```cpp
void process_collisions() 
{
	// 敌人与玩家的碰撞检测
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
    // 子弹与敌人、子弹与子弹的碰撞检测
    for (Bullet* bullet : bullet_list)
    {
        if (bullet->get_valid())
        {
            // 检测子弹与敌人
            if (bullet->collide_with_enemy)
            {
                for (Enemy* enemy : enemy_list)
                {
                    if (enemy->check_alive()
                        && check_collision_AABB(bullet->get_position(), bullet->get_size(), enemy->get_position(), enemy->get_size())
                        )
                    {
                        enemy->hp_loss(bullet->get_damage());
                        bullet->on_collide();
                    }
                }
            }
            // 检测子弹与其他子弹
            if (bullet->collide_with_bullet)
            {
                for (Bullet* other_bullet : bullet_list)
                {
                    // 避免检测自身
                    if (other_bullet != bullet && other_bullet->get_valid())
                    {
                        if (check_collision_AABB(bullet->get_position(), bullet->get_size(),
                            other_bullet->get_position(), other_bullet->get_size()))
                        {other_bullet->on_collide();}
                    }
                }
            }
			// 检测子弹与玩家
            if (bullet->collide_with_player)
            {
                if (check_collision_point(player->get_position(), player->get_size(),bullet->get_position()))
                {
                    player->hp_loss(bullet->get_damage());
					          bullet->on_collide();
                }
            }
        }
    }
}
```

### 2.7 粒子效果的实现

定义了particle，如下
```cpp
// 定义粒子结构体
struct Particle {
    Vector2 position;     // 当前位置
    int current_radius;   // 当前半径
    COLORREF color;       // 固定颜色
    Vector2 velocity;     // 运动速度
    int decrease_rate;    // 每帧半径衰减量
};
```

统一由粒子系统 ParticleSystem 负责管理和更新粒子效果。在 GameScene 的 on_update 方法中调用 particle_system->UpdateAll() 进行更新，在 on_draw 方法中调用 particle_system->DrawAll() 进行绘制。

```cpp
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
```

### 2.8 事件的实现

在EventSelection中的on_enter方法中实现随机选择事件，并对player进行修改。

```cpp
void on_enter()
{
    selected_event_index = rand() % num;


    switch (selected_event_index) {
    case 0: // 攻击力提升
        player->change_damage(15);
        current_img = &img_buff_damage;
        blessing_name = _T("攻击力提升");
        break;
    case 1: // 攻击速度提升
        player->attack_cd -= 100;
        if (player->attack_cd < 60) player->attack_cd = 60;
        current_img = &img_buff_attackcd;
        blessing_name = _T("祝福:攻击冷却缩短");
        break;
    case 2: // 最大生命值提升
        player->hp_max += 60;
        current_img = &img_buff_hp;
        blessing_name = _T("祝福:最大生命值提升");
        break;
    case 3: //特殊事件，斩碎子弹
        player->can_collide_with_bullet = true;
        current_img = &img_buff_slash;
        blessing_name = _T("稀有祝福:子弹可以被击碎");
        num=num - 1;    //只能获得一次
    }
}
```

## 结论
通过本次大作业，成功实现了一个基于面向对象设计的小游戏。系统具有良好的扩展性和可维护性，通过多态、继承和封装等面向对象特性，有效组织了复杂的游戏逻辑。后续可进一步扩展敌人类型、添加更多技能系统和优化游戏性能。
