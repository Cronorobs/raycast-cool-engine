//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include "map.h"
#include "editor.h"
#include "player.h"
#include "button.h"
#include "sprite.h"
#include <math.h>

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
    sprites.push_back(Sprite(*this, 10));
    sprites.push_back(Sprite(*this, 12));
    sprites.push_back(Sprite(*this, 26));

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1) { mapMatrix.push_back(1); }
        else { mapMatrix.push_back(0); }
    }
}

void Map::Resize(int newSize)
{
    mapMatrix.clear();
    size = newSize;

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1) { mapMatrix.push_back(1); }
        else { mapMatrix.push_back(0); }
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
    float rayAngle;
    float rayNumber = 80;
    int lineWidth = 640 / rayNumber;
    vector <float> zBuffer = vector<float>();

    rayAngle = player->angle - DR * 40;

    if (rayAngle < 0) { rayAngle += 2 * PI; }
    if (rayAngle > 2 * PI) {  rayAngle -= 2 * PI; }

    for (int i = 0; i < rayNumber; i++)
    {
        Ray ray = player->CastRay(*this, rayAngle, colors, length);
        zBuffer.push_back(ray.distance);

        //Draw 2D Rays
        if (editor->state == 1)
        {
            glLineWidth(1);
            glBegin(GL_LINES);
            glVertex2i(player->x + editor->width / 4, player->y + 64);
            glVertex2i(ray.x + editor->width / 4, ray.y + 64);
            glEnd();
        }

        //Draw 3D Walls
        float coolAngle = player->angle - rayAngle;
        if (coolAngle < 0) { coolAngle += 2 * PI; }
        if (coolAngle > 2 * PI) { coolAngle -= 2 * PI; }

        float drawDistance = ray.distance * cos(coolAngle);
        float lineHeight = cellSize * 480 / drawDistance;
        if (lineHeight > 480) { lineHeight = 480; }
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
        if (rayAngle < 0) { rayAngle += 2 * PI; }
        if (rayAngle > 2 * PI) { rayAngle -= 2 * PI; }
    }

    if (editor->state == 2)
    {
        for (int i = 0; i < sprites.size(); i++) { sprites[i].Draw(player, lineWidth, zBuffer); }
    }
}