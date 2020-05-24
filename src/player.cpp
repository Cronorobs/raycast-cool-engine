//Author: Roberto Abad Jiménez
//Date: 02/05/2020
#include "imgui/imgui.h"
#include <GL/glut.h>
#include "player.h"
#include <math.h>
#include "map.h"

#define PI 3.1415926535
#define DR 0.0174533
#define P3 3 * PI/2
#define P2 PI/2

Player::Player(int x, int y)
{
    this->x = x;
    this->y = x;
    angle = 0;
    this->deltaX = cos(angle) * 5;
    this->deltaY = sin(angle) * 5;
}

Ray Player::CastRay(Map map, float rayAngle, std::vector<ImColor> colors)
{
    int mapX;
    int mapY;
    int depthOfField;
    float xOffset;
    float yOffset;
    int verticalTile;
    int horizontalTile;

    Ray ray;

    //Check Horizontal Lines
    depthOfField = 0;
    float horizontalDist = 100000;
    float horizontalX = x;
    float horizontalY = y;
    float aTan = -1 / tan(rayAngle);
    if (rayAngle > PI)
    {
        ray.y = (((int)y >> 6) << 6) - 0.0001f;
        ray.x = (y - ray.y) * aTan + x;
        yOffset = -map.cellSize;
        xOffset = -yOffset * aTan;
    }
    if (rayAngle < PI)
    {
        ray.y = (((int)y >> 6) << 6) + map.cellSize;
        ray.x = (y - ray.y) * aTan + x;
        yOffset = map.cellSize;
        xOffset = -yOffset * aTan;
    }
    if (rayAngle == 0 || rayAngle == PI)
    {
        ray.x = x;
        ray.y = y;
        depthOfField = map.size;
    }
    while (depthOfField < map.size)
    {
        mapX = (int)(ray.x) >> 6;
        mapY = (int)(ray.y) >> 6;
        ray.tile = mapY * map.size + mapX;
        horizontalTile = ray.tile;
        if (ray.tile > 0 && ray.tile < map.size * map.size && map.mapMatrix[ray.tile] > 0)
        {
            horizontalX = ray.x;
            horizontalY = ray.y;
            horizontalDist = Distance(x, y, horizontalX, horizontalY);
            depthOfField = map.size;
        }
        else
        {
            ray.x += xOffset;
            ray.y += yOffset;
            depthOfField += 1;
        }
    }

    //Check Vertical Lines
    depthOfField = 0;
    float verticalDist = 100000;
    float verticalX = x;
    float verticalY = y;
    float nTan = -tan(rayAngle);
    if (rayAngle > P2 && rayAngle < P3)
    {
        ray.x = (((int)x >> 6) << 6) - 0.0001f;
        ray.y = (x - ray.x) * nTan + y;
        xOffset = -map.cellSize;
        yOffset = -xOffset * nTan;
    }
    if (rayAngle < P2 || rayAngle > P3)
    {
        ray.x = (((int)x >> 6) << 6) + map.cellSize;
        ray.y = (x - ray.x) * nTan + y;
        xOffset = map.cellSize;
        yOffset = -xOffset * nTan;
    }
    if (rayAngle == 0 || rayAngle == PI)
    {
        ray.x = x;
        ray.y = y;
        depthOfField = map.size;
    }
    while (depthOfField < map.size)
    {
        mapX = (int)(ray.x) >> 6;
        mapY = (int)(ray.y) >> 6;
        ray.tile = mapY * map.size + mapX;
        verticalTile = ray.tile;
        if (ray.tile > 0 && ray.tile < map.size * map.size && map.mapMatrix[ray.tile] > 0)
        {
            verticalX = ray.x;
            verticalY = ray.y;
            verticalDist = Distance(x, y, verticalX, verticalY);
            depthOfField = map.size;
        }
        else
        {
            ray.x += xOffset;
            ray.y += yOffset;
            depthOfField += 1;
        }
    }

    if (verticalDist < horizontalDist)
    {
        ray.x = verticalX;
        ray.y = verticalY;
        ray.tile = verticalTile;
        if (ray.tile < 0 || ray.tile > map.size * map.size) { ray.tile = 0; }
        ray.distance = verticalDist;
        if (map.mapMatrix[ray.tile] > 0 && map.mapMatrix[ray.tile] < colors.size())
        {
            glColor3f(colors[map.mapMatrix[ray.tile]].Value.x, colors[map.mapMatrix[ray.tile]].Value.y, colors[map.mapMatrix[ray.tile]].Value.z);
        }
    }
    else
    {
        ray.x = horizontalX;
        ray.y = horizontalY;
        ray.tile = horizontalTile;
        if (ray.tile < 0 || ray.tile > map.size * map.size) { ray.tile = 0; }
        ray.distance = horizontalDist;
        if (map.mapMatrix[ray.tile] > 0 && map.mapMatrix[ray.tile] < colors.size())
        {
            glColor3f(colors[map.mapMatrix[ray.tile]].Value.x - .1f, colors[map.mapMatrix[ray.tile]].Value.y - .1f, colors[map.mapMatrix[ray.tile]].Value.z - .1f);
        }
    }

    return ray;
}

float Player::Distance(float aX, float aY, float bX, float bY)
{
    return sqrt(pow((bX - aX), 2) + pow((bY - aY), 2));
}
