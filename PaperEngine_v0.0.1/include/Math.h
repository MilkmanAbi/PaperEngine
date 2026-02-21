#pragma once

#include "Vector2.h"
#include <algorithm>

namespace Paper {

/**
 * Color - RGBA color representation
 */
struct Color {
    unsigned char r, g, b, a;

    Color() : r(255), g(255), b(255), a(255) {}
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}

    // Common colors
    static Color white() { return Color(255, 255, 255); }
    static Color black() { return Color(0, 0, 0); }
    static Color red() { return Color(255, 0, 0); }
    static Color green() { return Color(0, 255, 0); }
    static Color blue() { return Color(0, 0, 255); }
    static Color yellow() { return Color(255, 255, 0); }
    static Color cyan() { return Color(0, 255, 255); }
    static Color magenta() { return Color(255, 0, 255); }
    static Color transparent() { return Color(0, 0, 0, 0); }

    // Paper-themed colors
    static Color paper() { return Color(245, 245, 220); } // Beige/cream
    static Color cardboard() { return Color(205, 170, 125); }
    static Color parchment() { return Color(240, 230, 200); }
    static Color ink() { return Color(30, 30, 40); }

    Color withAlpha(unsigned char alpha) const {
        return Color(r, g, b, alpha);
    }

    Color lerp(const Color& other, float t) const {
        t = std::clamp(t, 0.0f, 1.0f);
        return Color(
            static_cast<unsigned char>(r + (other.r - r) * t),
            static_cast<unsigned char>(g + (other.g - g) * t),
            static_cast<unsigned char>(b + (other.b - b) * t),
            static_cast<unsigned char>(a + (other.a - a) * t)
        );
    }
};

/**
 * Rectangle - Axis-aligned bounding box
 */
struct Rectangle {
    float x, y, width, height;

    Rectangle() : x(0), y(0), width(0), height(0) {}
    Rectangle(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}
    Rectangle(const Vector2& position, const Vector2& size)
        : x(position.x), y(position.y), width(size.x), height(size.y) {}

    float left() const { return x; }
    float right() const { return x + width; }
    float top() const { return y; }
    float bottom() const { return y + height; }

    Vector2 position() const { return Vector2(x, y); }
    Vector2 size() const { return Vector2(width, height); }
    Vector2 center() const { return Vector2(x + width / 2, y + height / 2); }

    bool contains(const Vector2& point) const {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }

    bool intersects(const Rectangle& other) const {
        return !(right() < other.left() || left() > other.right() ||
                 bottom() < other.top() || top() > other.bottom());
    }

    Rectangle intersection(const Rectangle& other) const {
        float x1 = std::max(left(), other.left());
        float y1 = std::max(top(), other.top());
        float x2 = std::min(right(), other.right());
        float y2 = std::min(bottom(), other.bottom());

        if (x1 < x2 && y1 < y2) {
            return Rectangle(x1, y1, x2 - x1, y2 - y1);
        }
        return Rectangle();
    }
};

} // namespace Paper
