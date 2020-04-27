//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include "editor.h"
#include "button.h"
#include <math.h>
#include "map.h"

#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174533

Map::Map(int x, int y, int size, Editor *editor)
{
    xSize = x;
    ySize = y;
    cellSize = size;
    this->editor = editor;
    mapMatrix = vector<int>();

    for (int i = 0; i < xSize * ySize; i++)
    {
        if (i < 8 || i > 55 || i % 8 == 0 || i % 8 == 7)
        {
            mapMatrix.push_back(1);
        }
        else
        {
            mapMatrix.push_back(0);
        }
    }
    mapMatrix.push_back(0);
}

void Map::DrawMap2D(Color colors[])
{
    if (editor->state == 1)
    {
        for (int i = 0; i < ySize; i++)
        {
            for (int j = 0; j < xSize; j++)
            {
                int index = i * ySize + j;
                glColor3f(colors[mapMatrix[index]].r, colors[mapMatrix[index]].g, colors[mapMatrix[index]].b);

                glBegin(GL_QUADS);
                glVertex2i(editor->width / 4 + j * cellSize, i * cellSize + 64);
                glVertex2i(editor->width / 4 + j * cellSize, i * cellSize + cellSize + 64);
                glVertex2i(editor->width / 4 + j * cellSize + cellSize, i * cellSize + cellSize + 64);
                glVertex2i(editor->width / 4 + j * cellSize + cellSize, i * cellSize + 64);
                glEnd();
            }
        }
    }
}

void Map::DrawRays(Player *player, Color colors[])
{
    int mapX;
    int mapY;
    int mp;
    int verticalMp;
    int horizontalMp;
    int depthOfField;
    float rayX;
    float rayY;
    float rayAngle;
    float xOffset;
    float yOffset;
    float finalDistance;

    rayAngle = player->angle - DR * 30;

    if (rayAngle < 0)
    {
        rayAngle += 2 * PI;
    }
    if (rayAngle > 2 * PI)
    {
        rayAngle -= 2 * PI;
    }

    for (int i = 0; i < 60; i++)
    {
        //Check Horizontal Lines
        depthOfField = 0;
        float horizontalDist = 100000;
        float horizontalX = player->x;
        float horizontalY = player->y;
        float aTan = -1 / tan(rayAngle);
        if (rayAngle > PI)
        {
            rayY = (((int)player->y >> 6) << 6) - 0.0001f;
            rayX = (player->y - rayY) * aTan + player->x;
            yOffset = -cellSize;
            xOffset = -yOffset * aTan;
        }
        if (rayAngle < PI)
        {
            rayY = (((int)player->y >> 6) << 6) + cellSize;
            rayX = (player->y - rayY) * aTan + player->x;
            yOffset = cellSize;
            xOffset = -yOffset * aTan;
        }
        if (rayAngle == 0 || rayAngle == PI)
        {
            rayX = player->x;
            rayY = player->y;
            depthOfField = 8;
        }
        while (depthOfField < 8)
        {
            mapX = (int)(rayX) >> 6;
            mapY = (int)(rayY) >> 6;
            mp = mapY * xSize + mapX;
            horizontalMp = mp;
            if (mp > 0 && mp < xSize * ySize && mapMatrix[mp] > 0)
            {
                horizontalX = rayX;
                horizontalY = rayY;
                horizontalDist = Distance(player->x, player->y, horizontalX, horizontalY);
                depthOfField = 8;
            }
            else
            {
                rayX += xOffset;
                rayY += yOffset;
                depthOfField += 1;
            }
        }

        //Check Vertical Lines
        depthOfField = 0;
        float verticalDist = 100000;
        float verticalX = player->x;
        float verticalY = player->y;
        float nTan = -tan(rayAngle);
        if (rayAngle > P2 && rayAngle < P3)
        {
            rayX = (((int)player->x >> 6) << 6) - 0.0001f;
            rayY = (player->x - rayX) * nTan + player->y;
            xOffset = -cellSize;
            yOffset = -xOffset * nTan;
        }
        if (rayAngle < P2 || rayAngle > P3)
        {
            rayX = (((int)player->x >> 6) << 6) + cellSize;
            rayY = (player->x - rayX) * nTan + player->y;
            xOffset = cellSize;
            yOffset = -xOffset * nTan;
        }
        if (rayAngle == 0 || rayAngle == PI)
        {
            rayX = player->x;
            rayY = player->y;
            depthOfField = 8;
        }
        while (depthOfField < 8)
        {
            mapX = (int)(rayX) >> 6;
            mapY = (int)(rayY) >> 6;
            mp = mapY * xSize + mapX;
            verticalMp = mp;
            if (mp > 0 && mp < xSize * ySize && mapMatrix[mp] > 0)
            {
                verticalX = rayX;
                verticalY = rayY;
                verticalDist = Distance(player->x, player->y, verticalX, verticalY);
                depthOfField = 8;
            }
            else
            {
                rayX += xOffset;
                rayY += yOffset;
                depthOfField += 1;
            }
        }

        if (verticalDist < horizontalDist)
        {
            rayX = verticalX;
            rayY = verticalY;
            mp = verticalMp;
            if (mp < 0 || mp > 64)
            {
                mp = 0;
            }
            finalDistance = verticalDist;
            if (mapMatrix[mp] == 1)
            {
                glColor3f(0.f, 0.5f, 0.6f);
            }
            if (mapMatrix[mp] == 2)
            {
                glColor3f(1.f, 0.f, 0.5f);
            }
            if (mapMatrix[mp] == 3)
            {
                glColor3f(.0f, .9f, 0.5f);
            }
        }
        else
        {
            rayX = horizontalX;
            rayY = horizontalY;
            mp = horizontalMp;
            if (mp < 0 || mp > 64)
            {
                mp = 0;
            }
            finalDistance = horizontalDist;
            if (mapMatrix[mp] == 1)
            {
                glColor3f(0.f, 0.4f, 0.5f);
            }
            if (mapMatrix[mp] == 2)
            {
                glColor3f(0.9f, 0.f, 0.4f);
            }
            if (mapMatrix[mp] == 3)
            {
                glColor3f(.0f, .8f, 0.4f);
            }
        }

        //Draw 2D Rays
        if (editor->state == 1)
        {
            glLineWidth(1);
            glBegin(GL_LINES);
            glVertex2i(player->x + editor->width / 4, player->y + 64);
            glVertex2i(rayX + editor->width / 4, rayY + 64);
            glEnd();
        }

        //Draw 3D Walls
        float coolAngle = player->angle - rayAngle;
        if (coolAngle < 0)
        {
            coolAngle += 2 * PI;
        }
        if (coolAngle > 2 * PI)
        {
            coolAngle -= 2 * PI;
        }
        finalDistance *= cos(coolAngle);
        float lineHeight = cellSize * 512 / finalDistance;
        if (lineHeight > 512)
        {
            lineHeight = 512;
        }
        float lineOffset = 512 / 2 - lineHeight / 2;

        if (editor->state == 2)
        {
            glLineWidth(8.53f);
            glBegin(GL_LINES);
            glVertex2i(i * 8.53f + 275, lineOffset + 64);
            glVertex2i(i * 8.53f + 275, lineHeight + lineOffset + 64);
            glEnd();
        }

        rayAngle += DR;
        if (rayAngle < 0)
        {
            rayAngle += 2 * PI;
        }
        if (rayAngle > 2 * PI)
        {
            rayAngle -= 2 * PI;
        }
    }
}

float Map::Distance(float aX, float aY, float bX, float bY)
{
    return sqrt(pow((bX - aX), 2) + pow((bY - aY), 2));
}