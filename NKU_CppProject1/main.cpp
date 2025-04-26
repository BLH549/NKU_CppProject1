#include <iostream>
#include <graphics.h>

#include "scene.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "eventselection_scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "camera.h"
#include "vector2.h"

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* eventselection_scene = nullptr;
SceneManager scene_manager;

//素材的加载
//素材的声明
IMAGE img_menu_background;	// 主菜单背景图片

void initialize_resources()
{
	AddFontResourceEx(_T("resources/1Pix.ttf"), FR_PRIVATE, NULL);

	//加载主菜单背景图片
	loadimage(&img_menu_background, _T("resources/menu_background.png"));
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
    return 0;
}

