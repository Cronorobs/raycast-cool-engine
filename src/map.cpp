//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include "imgui/imgui.h"
#include "GL/glut.h"
#include "player.h"
#include "sprite.h"
#include <math.h>
#include "map.h"

#define PI 3.1415926535
#define DR 0.0174533
#define P2 PI/2
#define P3 3 * PI/2

Map::Map(int s, int cSize)
{
    size = s;
    cellSize = cSize;
    mapMatrix = std::vector<int>();
    eventMatrix = std::vector<int>();
    sprites.push_back(Sprite(*this, 10));
    sprites.push_back(Sprite(*this, 12));
    sprites.push_back(Sprite(*this, 26));

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1) { mapMatrix.push_back(1); }
        else { mapMatrix.push_back(0); }
        eventMatrix.push_back(0);
    }
}

void Map::Resize()
{
    mapMatrix.clear();
    eventMatrix.clear();

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1) { mapMatrix.push_back(1); }
        else { mapMatrix.push_back(0); }
        eventMatrix.push_back(0);
    }
}

void Map::DrawMap(Player * player, std::vector<ImColor> colors)
{
    DrawBackground();

    float rayAngle;
    float rayNumber = 640;
    int lineWidth = 640 / rayNumber;
    vector <float> zBuffer = vector<float>();

    rayAngle = player->angle - DR * 40;

    if (rayAngle < 0) { rayAngle += 2 * PI; }
    if (rayAngle > 2 * PI) {  rayAngle -= 2 * PI; }

    for (int i = 0; i < rayNumber; i++)
    {
        Ray ray = player->CastRay(*this, rayAngle, colors);
        zBuffer.push_back(ray.distance);

        //Draw 3D Walls
        float coolAngle = player->angle - rayAngle;
        if (coolAngle < 0) { coolAngle += 2 * PI; }
        if (coolAngle > 2 * PI) { coolAngle -= 2 * PI; }

        float drawDistance = ray.distance * cos(coolAngle);
        float lineHeight = cellSize * 480 / drawDistance;
        if (lineHeight > 480) { lineHeight = 480; }
        float lineOffset = 480 / 2 - lineHeight / 2;

        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glVertex2i(i * lineWidth + 221, lineOffset + 80);
        glVertex2i(i * lineWidth + 221, lineHeight + lineOffset + 80);
        glEnd();

        rayAngle += DR * 80 / rayNumber;
        if (rayAngle < 0) { rayAngle += 2 * PI; }
        if (rayAngle > 2 * PI) { rayAngle -= 2 * PI; }
    }

    //Draw Sprites
    for (int i = 0; i < sprites.size(); i++) { sprites[i].Draw(player, lineWidth, zBuffer); }
}

void Map::DrawBackground()
{
    //Ground
    glColor3f(.1f, .2f, .3f);
    glBegin(GL_QUADS);
    glVertex2i(220, 80);
    glVertex2i(860, 80);
    glVertex2i(860, 560);
    glVertex2i(220, 560);
    glEnd();

    //Sky
    glColor3f(0.7f, 0.9f, 1.f);
    glBegin(GL_QUADS);
    glVertex2i(220, 80);
    glVertex2i(860, 80);
    glVertex2i(860, 320);
    glVertex2i(220, 320);
    glEnd();
}