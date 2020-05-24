//Author: Roberto Abad Jiménez
//Date: 06/05/2020
#include "sprite.h"
#include "player.h"
#include "map.h"
#include<stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define RTOD 57.2958f

Sprite::Sprite(Map map, int cell)
{
    cellSize = map.cellSize;
    x = (floor(cell % map.size) - 1) * cellSize + cellSize / 2;
    y = floor(cell / map.size) * cellSize + cellSize / 2;
    distance = 0;
    visible = false;
}

void Sprite::Calculate(Player * p)
{
    float playerObjectAngle = atan2(y - p->y, x - p->x);
    float angleDif = p->angle - playerObjectAngle;

    if(angleDif < PI) { angleDif += 2.f * PI; }
    if(angleDif > PI) { angleDif -= 2.f * PI; }

    angleDif = abs(angleDif * RTOD);

    if(angleDif < 40) { visible = true; }
    else { visible = false; }

    distance = p->Distance(p->x, p->y, x, y);
}

void Sprite::Draw(Player * p, int lineWidth, vector<float> zBuffer)
{
    Calculate(p);

    if(visible)
    {
        float playerObjectAngle = atan2(y - p->y, x - p->x);
        float angleDif = p->angle - playerObjectAngle;
        float drawDistance = distance * cos(angleDif);

        float size = cellSize * .75f * 480 / drawDistance;
        if (size > 480) { size = 480; }

        float offset = 480 / 2 - size / 2;

        float x0 = tan(playerObjectAngle - p->angle) * 640;
        int x1 = (int)(540 + x0 - size / 2);
        x1 = x1 + lineWidth - (x1 % lineWidth);

        int start = (int)((x1 - 221) / lineWidth);
        int lenght = (int)(size / lineWidth);

        for (int i = 0; i < lenght; i++)
        {
            if(i * lineWidth + x1 > 221 - lineWidth && i * lineWidth + x1 < 864 && distance < zBuffer[i + start])
            {
                glColor3f(1.f, .5f, 0.f);
                glBegin(GL_LINES);
                glVertex2i(i * lineWidth + x1, offset + 80);
                glVertex2i(i * lineWidth + x1, offset + 80 + size);
                glEnd();
            }
        }
    }
}