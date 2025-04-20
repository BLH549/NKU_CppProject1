#pragma once
#include <graphics.h>
#include "camera.h"

//抽象的场景基类
//核心功能：进入，更新，绘制，处理输入，退出
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