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
在本项目中，存在多个界面，且需要在多个界面中频繁切换。所以借助面向对象的特性，采用场景管理器模式实现了高效、低耦合的场景切换系统。

#### 2.1.1 场景类的实现
为统一场景行为规范，首先设计了抽象基类 Scene，定义了其核心方法：
```cpp
class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    virtual void on_enter() {}
    virtual void on_update(int delta) {}
    virtual void on_draw() {}
    virtual void on_input(const ExMessage& msg) {}
    virtual void on_exit() {}

private:

};
```
该抽象类定义了场景的基本模式，具体场景子类通过继承并重写这些方法实现特定行为。
例如：
* MenuScene：实现游戏主菜单界面
* GameScene：实现游戏核心玩法逻辑
* EventSelectionScene：实现事件选择系统


#### 2.1.2 场景管理器实现

场景管理器 SceneManager 作为中央控制枢纽，负责场景的管理、切换和更新等操作。核心代码如下：

```cpp
extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* eventselection_scene;

class SceneManager
{
public:
    enum class SceneType
    {
        Menu,
        Game,
	EventSelection
    };

private:
    Scene* current_scene = nullptr;

public:
    SceneManager() = default;
    ~SceneManager() = default;

    void set_current_scene(Scene* scene)
    {
        current_scene = scene;
        current_scene->on_enter();
    }

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

    // 委托调用当前场景方法
    void on_update(int delta){current_scene->on_update(delta);}
    void on_draw(){current_scene->on_draw();}
    void on_input(const ExMessage& msg){current_scene->on_input(msg);}

```
该设计主要实现了以下两个功能：

* 场景切换：

	通过SceneManager的 switch_to 方法实现平滑过渡。例如在GameScene中，当每轮游戏时间结束时，计时器会执行如下的回调函数：
	```cpp
	//更新
	timer_game_round.set_timeout([&]() {
    	scene_manager.switch_to(SceneManager::SceneType::EventSelection);
   	 });
	```

* 统一接口：
  
  	SceneManager提供 on_update、on_draw、on_input 等统一调用接口。在游戏主循环中可以方便地使用Scenemanager的方法来调用当前场景的逻辑。
	例如在主循环中，通过下面代码来实现当前场景的更新逻辑。
	```cpp
	//更新
	static DWORD last_tick_time = GetTickCount();
	DWORD current_tick_time = GetTickCount();
	DWORD delta_tick = current_tick_time - last_tick_time;
	scene_manager.on_update(delta_tick);
	```
  

### 2.2 动画类的实现

在本项目中，Animation 类负责管理游戏对象的动画。它采用帧序列 + 计时器的模式实现动画播放：通过std::vector存储动画帧序列，利用Timer类控制帧播放间隔。
每到设定时间，Timer触发回调，切换至下一帧；若播放到序列末尾，根据is_loop属性判断是否循环播放或触发结束回调。

```cpp
// 核心播放逻辑
timer.set_timeout([&]() {
    idx_frame++;
    if (idx_frame >= frame_list.size()) {
        idx_frame = is_loop ? 0 : frame_list.size() - 1;
        if (!is_loop && on_finished) {
            on_finished();
        }
    }
});
```
此外，该类提供了灵活的资源管理接口，支持从单张整图按区域切割添加帧，或从图像集批量导入帧。
更新与渲染流程也进行了封装，on_update方法驱动计时器，on_render方法根据当前帧索引完成图像绘制，方便与游戏主循环集成 。

### 2.3 玩家的实现
Player 类是游戏的核心角色。以下是核心功能介绍：
1. **基础属性** ：
	```cpp
 	Vector2 size;                      //角色尺寸
	Vector2 position;                // 角色位置,中心点
	Vector2 velocity;                   //角色速度
	int damage = 20; //角色伤害值
	int hp_max = 70; //角色最大血量
	int hp = 70; //角色血量
	int attack_cd = 500; //角色攻击冷却时间
	const float run_velocity = 0.25f;//跑动速度
	bool can_collide_with_bullet = false; //是否可以碰撞子弹
	```
2. **动画系统** :
   	持有 4 个动画（左右移动 / 左右待机）根据移动状态自动切换动画。
   	```cpp
 	if (is_moving)
	{
   	 	if (direction != 0)
    		{
        		is_facing_right = direction > 0;
   		}
    		current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
	}	
	else
	{
    		current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
	}
	current_animation->on_update(delta);
 	```
3.  **移动攻击** ：
   
   	WASD 四向移动,按J进行攻击
    
4. **状态管理**：
   - 通过计时器控制状态持续时间
   - 受伤后短暂无敌
   - 攻击冷却(一段时间内无法再次攻击)
  
5. **核心方法**:

   主要提供了on_input,on_updaee,on_render来处理输入，更新 ，绘制逻辑。


### 2.4 敌人的实现
在本游戏项目中，通过继承基类 Enemy 实现了不同类型敌人的统一管理与差异化行为。
目前已实现的敌人类型包括 Orc 和 Shaman，由统一的 enemy_list 进行管理和更新，确保了游戏逻辑的一致性和可扩展性。


### 2.4.1 敌人基类设计

Enemy 基类定义了所有敌人类型必须具备的通用属性和行为接口。
这些基础属性包括位置、速度、动画状态等，同时也定义了敌人与玩家交互的基本方式。
基类中enemy的动画系统和方法接口与 Player 类有相似之处，因此在此不再赘述。

其中构造函数实现了敌人在地图边缘的随机生成逻辑：
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
on_run方法实现朝玩家移动
```cpp
void on_run(int delta, const Player* player)
{
    
    Vector2 dir = { player->get_position().x - position.x, player->get_position().y - position.y };
    dir_player_normalized = dir.normalize();
    
    position.x += (int)(run_velocity * dir_player_normalized.x * delta);
    position.y += (int)(run_velocity * dir_player_normalized.y * delta);

}
```
尽管所有敌人共享基础行为，但不同敌人之间攻击方法各异。
例如，Orc 仅能通过碰撞造成伤害，而 Shaman 不仅能碰撞攻击，还能发射远程子弹。受限于篇幅，不作展开

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

当敌人受伤或死亡时，会产生一定量的粒子。这些粒子由统一由粒子系统 ParticleSystem 负责管理和更新粒子效果。

在 GameScene 的 on_update 方法中调用 particle_system->UpdateAll() 进行更新，在 on_draw 方法中调用 particle_system->DrawAll() 进行绘制。

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
此外每个事件都有对应的图片与文字表述，在on_draw方法中实现绘制

## 结论
通过本次大作业，成功实现了一个基于面向对象设计的小游戏。系统具有良好的扩展性和可维护性，通过多态、继承和封装等面向对象特性，有效组织了复杂的游戏逻辑。后续可进一步扩展敌人类型、添加更多技能系统和优化游戏性能。
