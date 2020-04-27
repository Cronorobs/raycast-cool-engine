//Author: Roberto Abad Jiménez
//Date: 26/04/2020
#include "editor.h"
#include "mouse.h"
#include "button.h"

//Constructor of the Editor class
Editor::Editor(Mouse *mouse, int width, int height)
{
    state = 0;
    currentButton = 0;
    this->mouse = mouse;
    this->width = width;
    this->height = height;
    buttons = vector<Button *>();
    font = GLUT_BITMAP_HELVETICA_18;
}

void Editor::DrawBackground()
{
    if (state == 2)
    {
        //Floor
        Color brown = {0.2f, 0.1f, 0.1f};
        DrawBox(width / 4, 64, 512, 512, brown);

        //Sky
        Color lightBlue = {0.7f, 0.9f, 1.f};
        DrawBox(width / 4, 64, 512, 256, lightBlue);
    }
}

void Editor::DrawEditor()
{
    //Common part
    Color gray = {.4f, .4f, .4f};
    Color darkGray = {.25f, .25f, .25f};

    DrawBox(width / 4 - 12, 52, 536, 536, gray);
    DrawBox(width / 4 - 2, 62, 516, 516, darkGray);

    //Map editor
    if (state == 0)
    {
        DrawButtons(0, 67);
    }
    //2D and 3D views
    else
    {
        DrawButtons(0, 3);
    }
}

//Draw a (c) color box on a (x,y) position with (w,h) dimensions
void Editor::DrawBox(int x, int y, int w, int h, Color c)
{
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

//Draw a (text) string on a (x,y) position
void Editor::DrawText(string text, int x, int y)
{
    glColor3f(1.f, 1.f, 1.f);
    glRasterPos2i(x, y);

    for (int i = 0; i < text.length(); i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

//Draw the range of buttons [from - to] of the buttons vector
void Editor::DrawButtons(int from, int to)
{
    for (int i = from; i < to; i++)
    {
        buttons[i]->DrawButton(*this);
    }
}

//Execute the callback of a button if it's highlighted
void Editor::CheckButtons(int from, int to)
{
    for (int i = from; i < to; i++)
    {
        if (buttons[i]->highlighted)
        {
            buttons[i]->callback();
        }
    }
}

//Change the state of a button if the mouse is over it
void Editor::CheckHighlighted(int from, int to)
{
    for (int i = from; i < to; i++)
    {
        if (buttons[i]->IsMouseOver(mouse->x, mouse->y))
        {
            currentButton = buttons[i]->id;
            buttons[i]->highlighted = true;
        }
        else
        {
            buttons[i]->highlighted = false;
        }
    }
}