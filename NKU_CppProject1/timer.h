#pragma once
#include <functional>

//定时器
//功能：设置等待时间间隔，设置回调函数，暂停和恢复定时器，单次触发和多次触发
class Timer
{
private:
    int pass_time = 0;     // 已过时间
    int wait_time = 0;     // 等待时间
    bool paused = false;   // 是否暂停
    bool shotted = false;  // 是否触发
    bool one_shot = false; // 单次触发
    std::function<void()> callback;  // 触发回调

public:
    Timer() = default;
    ~Timer() = default;

    //重装
    void restart()
    {
        pass_time = 0;
        shotted = false;
    }

    void set_wait_time(int val)
    {
        wait_time = val;
    }
    //是否单次触发
    void set_one_shot(bool flag)
    {
        one_shot = flag;
    }

    //设置回调函数
    void set_callback(std::function<void()> callback)
    {
        this->callback = callback;
    }

    //暂停
    void pause()
    {
        paused = true;
    }
    //恢复
    void resume()
    {
        paused = false;
    }


    //更新
    void on_update(int delta)
    {
        if (paused)
            return;

        pass_time += delta;
        if (pass_time >= wait_time)
        {
            if ((!one_shot || (one_shot && !shotted)) && callback)
                callback();
            shotted = true;
            pass_time = 0;
        }
    }



};

