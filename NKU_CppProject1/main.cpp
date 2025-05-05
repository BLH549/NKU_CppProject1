#include <iostream>
#include <graphics.h>
#include<vector>


#include "vector2.h"
#include "camera.h"
#include "timer.h"
#include "util.h"
#include "atlas.h"
#include "animation.h"
#include "status_bar.h"
#include "player.h"   
#include "enemy.h"   
#include "scene.h"
#include "bullet.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "eventselection_scene.h"
#include "scene_manager.h"

#pragma comment(lib,"Winmm.lib")

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* eventselection_scene = nullptr;
SceneManager scene_manager;

bool is_debug = false; //调试模式开关
int round_num = 0;

Player* player = nullptr;
std::vector<Enemy*> enemy_list;
std::vector<Bullet*> bullet_list;


//素材的加载
//素材的声明
IMAGE img_menu_background;	// 主菜单背景图片
IMAGE img_game_background;	// 游戏场景背景图片
IMAGE img_player_run_right;
IMAGE* img_player_run_left = nullptr;
IMAGE img_player_idle_right;
IMAGE* img_player_idle_left=nullptr;
IMAGE img_orc_run_right;
IMAGE* img_orc_run_left = nullptr;
IMAGE img_shaman_run_right;
IMAGE* img_shaman_run_left;

IMAGE img_player_avatar;

IMAGE shaman_bullet;
Atlas atlas_slash_left, atlas_slash_right;
Atlas atlas_shaman_bullet_break;



//资源加载与动画初始化
void initialize_resources()
{
	//加载字体
	AddFontResourceEx(_T("resources/1Pix.ttf"), FR_PRIVATE, NULL);

	//加载主菜单背景图片
	loadimage(&img_menu_background, _T("resources/menu_background.png"));

	//加载游戏场景背景图片
	loadimage(&img_game_background, _T("resources/game_background.png"));

	//初始化角色移动动画
	loadimage(&img_player_run_right, _T("resources/player_run.png"));
	img_player_run_left = new IMAGE();
	flip_image(&img_player_run_right, img_player_run_left, 6);

	//初始化角色静止动画
	loadimage(&img_player_idle_right, _T("resources/player_idle.png"));
	img_player_idle_left = new IMAGE();
	flip_image(&img_player_idle_right, img_player_idle_left, 4);

	//初始化角色头像：
	loadimage(&img_player_avatar, _T("resources/player_avatar.png"));

	//初始化orc移动动画
	loadimage(&img_orc_run_right, _T("resources/orc_run.png"));
	img_orc_run_left = new IMAGE();
	flip_image(&img_orc_run_right, img_orc_run_left, 6);

	//初始化shaman移动动画
	loadimage(&img_shaman_run_right, _T("resources/shaman_run.png"));
	img_shaman_run_left = new IMAGE();
	flip_image(&img_shaman_run_right, img_shaman_run_left, 6);

	//初始化斩击动画
	atlas_slash_right.load_from_file(_T("resources/slash_%d.png"), 6);
	flip_atlas(atlas_slash_right, atlas_slash_left);


	//初始化哥布林子弹
	loadimage(&shaman_bullet, _T("resources/shaman_bullet.png"));
	atlas_shaman_bullet_break.load_from_file(_T("resources/shaman_bullet_break_%d.png"), 3);

	mciSendString(_T("open resources/bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
	
	mciSendString(_T("open resources/slash.mp3 alias slash"), NULL, 0, NULL);
	mciSendString(_T("open resources/being_hurt.mp3 alias being_hurt"), NULL, 0, NULL);


}

void unload_resources()
{
	//释放资源
	delete img_player_run_left;
	delete img_player_idle_left;
	delete img_orc_run_left;
	delete img_shaman_run_left;
	//卸载字体
	RemoveFontResourceEx(_T("resources/1Pix.ttf"), FR_PRIVATE, NULL);
	

}

int main()
{
	//游戏的初始化部分
	ExMessage msg;
	const int FPS = 60;
	initgraph(1280, 720, EW_SHOWCONSOLE);
	//设置字体样式
	settextstyle(28, 0, _T("IPix"));
	//文本背景色为透明
	setbkmode(TRANSPARENT);

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	eventselection_scene = new EventSelectionScene();

	//初始化资源
	initialize_resources();

	scene_manager.set_current_scene(menu_scene);


	//游戏主循环
	BeginBatchDraw();

	while (true)
	{

		DWORD frame_start_time = GetTickCount();

		//消息处理
		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}

		//逻辑更新
		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick);
		last_tick_time = current_tick_time;


		//渲染
		cleardevice();

		scene_manager.on_draw();

		FlushBatchDraw();


		//锁定帧率
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - frame_delta_time);
		}


	}

	EndBatchDraw();
	unload_resources();
	return 0;
}

