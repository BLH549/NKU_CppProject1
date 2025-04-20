#pragma once
#include <graphics.h>
#include "camera.h"

//����ĳ�������
//���Ĺ��ܣ����룬���£����ƣ��������룬�˳�
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