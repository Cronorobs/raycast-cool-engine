//Author: Roberto Abad Jiménez
//Date: 26/04/2020
#include "button.h"
#include "editor.h"

Button::Button(int x, int y, int w, int h, int id, string text, Color color, ButtonCallback callback)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->id = id;
    this->text = text;
    this->color = color;
    this->callback = callback;
    this->highlighted = false;
}

void Button::DrawButton(Editor editor)
{
    if (highlighted)
    {
        Color light = {color.r + .1f, color.g + .1f, color.b + .1f};
        editor.DrawBox(x, y, w, h, light);
    }
    else
    {
        editor.DrawBox(x, y, w, h, color);
    }
    editor.DrawText(text, x + 8, y + 22);
}

bool Button::IsMouseOver(int mouseX, int mouseY)
{
    if (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h)
    {
        return true;
    }
    else
    {
        return false;
    }
}