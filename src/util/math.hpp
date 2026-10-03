#pragma once
#include <string>
#include <iostream>
#include <math.h>
#include <imgui.h>

struct Vec4
{
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")";
    }
};

struct Vec3
{
    float x = 0.0f, y = 0.0f, z = 0.0f;

    std::string ToString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
    Vec3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }
    float& operator[](int index) {
        return (&x)[index];
    }
    const float& operator[](int index) const {
        return (&x)[index];
    }
    void Normalize() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length != 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }
    float Distance(const Vec3& other) const {
        float dx = other.x - x;
        float dy = other.y - y;
        float dz = other.z - z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

struct Vec2
{
    float x = 0.0f, y = 0.0f;

    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    operator ImVec2() const {
        return ImVec2(x, y);
    }

    Vec2 operator+(float scalar) const {
        return { x + scalar, y + scalar };
    }

    Vec2 operator-(float scalar) const {
        return { x - scalar, y - scalar };
    }
};

using Vector4 = Vec4;
using Vector3 = Vec3;
using Vector2 = Vec2;

bool WorldToScreen(const Vec3& in, Vec2& out);