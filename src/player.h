//Author: Roberto Abad Jiménez
//Date: 02/05/2020
#include <vector>
struct ImColor;
class Map;

struct Ray
{
    float x;
    float y;
    int tile;
    float distance;
};

class Player
{
public:
    int x;
    int y;
    float deltaX;
    float deltaY;
    float angle;

    Player(int x, int y);

    float Distance(float aX, float aY, float bX, float bY);
    Ray CastRay(Map map, float rayAngle, std::vector<ImColor> colors);
};
