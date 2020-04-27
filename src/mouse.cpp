//Author: Roberto Abad Jiménez
//Date: 26/04/2020
#include "editor.h"
#include "mouse.h"

Mouse::Mouse()
{
    x = 0;
    y = 0;
    rPressed = false;
    lPressed = false;
}

void Mouse::OnClick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            lPressed = true;
            break;
        case GLUT_RIGHT_BUTTON:
            rPressed = true;
            break;
        }
    }
    else
    {
        switch (button)
        {
        case GLUT_LEFT_BUTTON:
            lPressed = false;
            break;
        case GLUT_RIGHT_BUTTON:
            rPressed = false;
            break;
        }
    }
}

void Mouse::OnMove(int x, int y)
{
    this->x = x;
    this->y = y;
}