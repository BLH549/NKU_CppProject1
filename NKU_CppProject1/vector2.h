#pragma once
#include<cmath>

//二维向量类
//实现了常见的向量运算，包括加法、减法、点乘、数乘、长度计算和归一化为单位向量。
class Vector2
{
public:
    float x = 0;
    float y = 0;

public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(float x, float y)
        : x(x), y(y) {
    }

    Vector2 operator+(const Vector2& vec) const
    {
        return Vector2(x + vec.x, y + vec.y);
    }

    void operator+=(const Vector2& vec)
    {
        x += vec.x, y += vec.y;
    }

    Vector2 operator-(const Vector2& vec) const
    {
        return Vector2(x - vec.x, y - vec.y);
    }

    void operator-=(const Vector2& vec)
    {
        x -= vec.x, y -= vec.y;
    }


    float operator*(const Vector2& vec) const
    {
        return x * vec.x + y * vec.y;
    }


    Vector2 operator*(float val) const
    {
        return Vector2(x * val, y * val);
    }

    void operator*=(float val)
    {
        x *= val, y *= val;
    }

    float length()
    {
        return sqrt(x * x + y * y);
    }

    //归一化为单位向量
    Vector2 normalize()
    {
        float len = length();
        if (len == 0)
            return Vector2(0, 0);
        return Vector2(x / len, y / len);
    }
};
