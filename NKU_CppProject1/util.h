#pragma once

#include<graphics.h>
#include "atlas.h"

#pragma comment(lib,"MSIMG32.LIB")

struct Rect
{
    int x, y;   //左上角坐标
    int w, h;   //宽高
};

//单个图片的反转
inline void flip_image(IMAGE* src, IMAGE* dst)
{
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int idx_src = y * w + x;
            int idx_dst = y * w + (w - x - 1);
            dst_buffer[idx_dst] = src_buffer[idx_src];
        }
    }
}

//反转

void flip_atlas(Atlas& src, Atlas& dst)
{
    dst.clear();
    for (int i = 0; i < src.get_size(); i++)
    {
        IMAGE img_flpipped;
        flip_image(src.get_image(i), &img_flpipped);
        dst.add_image(img_flpipped);
    }
}

//长条素材的反转
void flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h)
{
    int w = src_image->getwidth();
    int h = src_image->getheight();
    int w_frame = w / num_h;
    Resize(dst_image, w, h);
    DWORD* src_buffer = GetImageBuffer(src_image);
    DWORD* dst_buffer = GetImageBuffer(dst_image);
    for (int i = 0; i < num_h; i++)
    {
        int x_left = i * w_frame;
        int x_right = (i + 1) * w_frame;
        for (int y = 0; y < h; y++)
        {
            for (int x = x_left; x < x_right; x++)
            {
                int dx = x - x_left; // 块内相对X坐标
                int flipped_x = x_right - 1 - dx; // 翻转后的块内X坐标
                int idx_src = y * w + x;
                int idx_dst = y * w + flipped_x; // 正确计算目标索引
                dst_buffer[idx_dst] = src_buffer[idx_src];

            }
        }
    }
}

//图片绘制
inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//带摄像机的图片绘制
inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    const Vector2& pos_camera = camera.get_position();
    AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - pos_camera.x), (int)(dst_y - pos_camera.y),
        w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

//条状素材的每帧绘制
inline void putimage_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
        GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
        rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
    const Vector2& pos_camera = camera.get_position();
    line((int)(x1 - pos_camera.x), (int)(y1 - pos_camera.y), (int)(x2 - pos_camera.x), (int)(y2 - pos_camera.y));
}


//显示白色轮廓
inline void sketch_image(IMAGE* src, IMAGE* dst)
{
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dst, w, h);
    DWORD* src_buffer = GetImageBuffer(src);
    DWORD* dst_buffer = GetImageBuffer(dst);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int idx = y * w + x;
            dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (src_buffer[idx] & 0xFF000000);
        }
    }
}



