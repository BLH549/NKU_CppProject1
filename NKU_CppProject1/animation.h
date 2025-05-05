#pragma once

#include "atlas.h"
#include "util.h"
#include "vector2.h"
#include "timer.h"

#include <graphics.h>
#include <functional>
#include <vector>

class Animation
{
private:
    //图片素材与裁剪矩形
    struct Frame
    {
        Rect rect_src;
        IMAGE* image = nullptr;

        Frame() = default;
        Frame(IMAGE* image, const Rect& rect_src)
            : image(image), rect_src(rect_src) {
        }

        ~Frame() = default;
    };

private:

    Timer timer;    //计时器，处理动画循环播放或单次播放
    Vector2 position;   //中心点
    bool is_loop = true;    //是否循环播放
    size_t idx_frame = 0;   //当前帧索引
    std::vector<Frame> frame_list;  //动画帧列表
    std::function<void()> on_finished;  //动画结束回调函数

public:
    Animation()
    {

        
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
        );
    }

    ~Animation()
    {
    }

    void reset()
    {
        timer.restart();
        idx_frame = 0;
    }

    void set_position(const Vector2& position)
    {
        this->position = position;
    }

    void set_loop(bool is_loop)
    {
        this->is_loop = is_loop;
    }

    void set_interval(float interval)
    {
        timer.set_wait_time(interval);
    }

    void set_on_finished(std::function<void()> on_finished)
    {
        this->on_finished = on_finished;
    }

    void add_frame(IMAGE* image, int num_h)
    {
        int width = image->getwidth();
        int height = image->getheight();
        int width_frame = width / num_h;
        for (int i = 0; i < num_h; i++)
        {
            Rect rect_src;
            rect_src.x = i * width_frame, rect_src.y = 0;
            rect_src.w = width_frame, rect_src.h = height;
            frame_list.emplace_back(image, rect_src);
        }
    }

    void add_frame(Atlas* atlas)
    {
        for (int i = 0; i < atlas->get_size(); i++)
        {
            IMAGE* image = atlas->get_image(i);
            int width = image->getwidth();
            int height = image->getheight();

            Rect rect_src;
            rect_src.x = 0, rect_src.y = 0;
            rect_src.w = width, rect_src.h = height;

            frame_list.emplace_back(image, rect_src);
        }
    }

    void on_update(float delta)
    {
        timer.on_update(delta);
    }

    void on_render(float scale)
    {
        const Frame& frame = frame_list[idx_frame];

        Rect rect_dst;
        rect_dst.x = (int)position.x - frame.rect_src.w / 2;
        rect_dst.y = (int)position.y - frame.rect_src.h / 2;
        rect_dst.w = (int)(scale * frame.rect_src.w), rect_dst.h = (int)(scale * frame.rect_src.h);

        putimage_ex(frame.image, &rect_dst, &frame.rect_src);
    }


};
