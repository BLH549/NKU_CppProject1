#pragma once

#include "scene.h"

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

    void on_update(int delta)
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

private:
    Scene* current_scene = nullptr;
};