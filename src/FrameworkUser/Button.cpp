#include "Button.h"
#include <iostream>

Button::Button(int width, int height)
{
    this->width = width;
    this->height = height;
    color = WHITE;
}

void Button::setPosition(Vector2 position)
{
    this->position = position;
}

Vector2 Button::getPosition()
{
    return position;
}

void Button::setColor(Color color)
{
    this->color = color;
}

void Button::Draw()
{
    DrawRectangle(position.x, position.y, width, height, color);
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = { position.x, position.y, static_cast<float>(width), static_cast<float>(height)};

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
    {
        return true;
    }
    return false;
}