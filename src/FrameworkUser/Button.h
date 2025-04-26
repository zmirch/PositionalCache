#pragma once
#include <raylib.h>

class Button
{
public:
    Button(int width, int height);
    Button(Vector2 position, int width, int height, Color color) : position(position), width(width), height(height), color(color) {}
    void setPosition(Vector2 position);
    Vector2 getPosition();
    void setColor(Color color);
    void Draw();
    bool isPressed(Vector2 mousePos, bool mousePressed);
private:
    Vector2 position;
    Color color;
    int width;
    int height;
};