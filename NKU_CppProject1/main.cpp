#include <iostream>
#include <graphics.h>
#include<vector>
#include <tchar.h>
#include <windows.h>

#include "scene.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "eventselection_scene.h"
#include "scene_manager.h"
#include "timer.h"
#include "camera.h"
#include "vector2.h"
#include "util.h"
#include "atlas.h"
#include "animation.h"
#include "player.h"
#include "enemy.h"

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* eventselection_scene = nullptr;
SceneManager scene_manager;

bool is_debug = false; //����ģʽ����

Player* player = nullptr;

//�زĵļ���
//�زĵ�����
IMAGE img_menu_background;	// ���˵�����ͼƬ
IMAGE img_game_background;	// ��Ϸ��������ͼƬ
IMAGE img_player_run_right;
IMAGE* img_player_run_left = nullptr;
IMAGE img_player_idle_right;
IMAGE* img_player_idle_left;
IMAGE img_orc_run_right;
IMAGE* img_orc_run_left = nullptr;


//��Դ�����붯����ʼ��
void initialize_resources()
{
	//��������
	AddFontResourceEx(_T("resources/1Pix.ttf"), FR_PRIVATE, NULL);

	//�������˵�����ͼƬ
	loadimage(&img_menu_background, _T("resources/menu_background.png"));

	//������Ϸ��������ͼƬ
	loadimage(&img_game_background, _T("resources/game_background.png"));

	//��ʼ����ɫ�ƶ�����
	loadimage(&img_player_run_right, _T("resources/player_run.png"));
	img_player_run_left = new IMAGE();
	flip_image(&img_player_run_right, img_player_run_left, 6);

	//��ʼ����ɫ��ֹ����
	loadimage(&img_player_idle_right, _T("resources/player_idle.png"));
	img_player_idle_left = new IMAGE();
	flip_image(&img_player_idle_right, img_player_idle_left, 4);

	//��ʼ���粼���ƶ�����
	loadimage(&img_orc_run_right, _T("resources/orc_run.png"));
	img_orc_run_left = new IMAGE();
	flip_image(&img_orc_run_right, img_orc_run_left, 6);
}

void unload_resources()
{
	//�ͷ���Դ
	delete img_player_run_left;
	delete img_player_idle_left;
	delete img_orc_run_left;
	//ж������
	RemoveFontResourceEx(_T("resources/1Pix.ttf"), FR_PRIVATE, NULL);
}

int main()
{
	//��Ϸ�ĳ�ʼ������
	ExMessage msg;
	const int FPS = 60;
	initgraph(1280, 720, EW_SHOWCONSOLE);
	//����������ʽ
	settextstyle(28, 0, _T("IPix"));
	//�ı�����ɫΪ͸��
	setbkmode(TRANSPARENT);

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	eventselection_scene = new EventSelectionScene();

	//��ʼ����Դ
	initialize_resources();

	scene_manager.set_current_scene(menu_scene);


	//��Ϸ��ѭ��
	BeginBatchDraw();

	while (true)
	{

		DWORD frame_start_time = GetTickCount();

		//��Ϣ����
		while (peekmessage(&msg))
		{
			scene_manager.on_input(msg);
		}

		//�߼�����
		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick);
		last_tick_time = current_tick_time;


		//��Ⱦ
		cleardevice();

		scene_manager.on_draw();

		FlushBatchDraw();


		//����֡��
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

