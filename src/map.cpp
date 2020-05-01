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

Map::Map(int s, int cSize, Editor *editor)
{
    size = s;
    cellSize = cSize;
    this->editor = editor;
    mapMatrix = vector<int>();

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1)
        {
            mapMatrix.push_back(1);
        }
        else
        {
            mapMatrix.push_back(0);
        }
    }
}

void Map::Resize(int newSize)
{
    mapMatrix.clear();
    size = newSize;

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1)
        {
            mapMatrix.push_back(1);
        }
        else
        {
            mapMatrix.push_back(0);
        }
    }
}

void Map::DrawMap2D(Color colors[])
{
    if (editor->state == 1)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int index = i * size + j;
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

void Map::DrawRays(Player *player, Color colors[], int length)
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
    float rayNumber = 80;
    int lineWidth = 640 / rayNumber;

    rayAngle = player->angle - DR * 40;

    if (rayAngle < 0)
    {
        rayAngle += 2 * PI;
    }
    if (rayAngle > 2 * PI)
    {
        rayAngle -= 2 * PI;
    }

    for (int i = 0; i < rayNumber; i++)
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
            depthOfField = size;
        }
        while (depthOfField < size)
        {
            mapX = (int)(rayX) >> 6;
            mapY = (int)(rayY) >> 6;
            mp = mapY * size + mapX;
            horizontalMp = mp;
            if (mp > 0 && mp < size * size && mapMatrix[mp] > 0)
            {
                horizontalX = rayX;
                horizontalY = rayY;
                horizontalDist = Distance(player->x, player->y, horizontalX, horizontalY);
                depthOfField = size;
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
            depthOfField = size;
        }
        while (depthOfField < size)
        {
            mapX = (int)(rayX) >> 6;
            mapY = (int)(rayY) >> 6;
            mp = mapY * size + mapX;
            verticalMp = mp;
            if (mp > 0 && mp < size * size && mapMatrix[mp] > 0)
            {
                verticalX = rayX;
                verticalY = rayY;
                verticalDist = Distance(player->x, player->y, verticalX, verticalY);
                depthOfField = size;
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
            if (mp < 0 || mp > size * size)
            {
                mp = 0;
            }
            finalDistance = verticalDist;
            if (mapMatrix[mp] > 0 && mapMatrix[mp] < length)
            {
                glColor3f(colors[mapMatrix[mp]].r, colors[mapMatrix[mp]].g, colors[mapMatrix[mp]].b);
            }
        }
        else
        {
            rayX = horizontalX;
            rayY = horizontalY;
            mp = horizontalMp;
            if (mp < 0 || mp > size * size)
            {
                mp = 0;
            }
            finalDistance = horizontalDist;
            if (mapMatrix[mp] > 0 && mapMatrix[mp] < length)
            {
                glColor3f(colors[mapMatrix[mp]].r - .1f, colors[mapMatrix[mp]].g - .1f, colors[mapMatrix[mp]].b - .1f);
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
        float lineHeight = cellSize * 480 / finalDistance;
        if (lineHeight > 480)
        {
            lineHeight = 480;
        }
        float lineOffset = 480 / 2 - lineHeight / 2;

        if (editor->state == 2)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            glVertex2i(i * lineWidth + 224, lineOffset + 80);
            glVertex2i(i * lineWidth + 224, lineHeight + lineOffset + 80);
            glEnd();
        }

        rayAngle += DR * 80 / rayNumber;
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