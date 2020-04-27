//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include <vector>
using namespace std;
class Editor;
struct Color;

struct Player
{
public:
    int x;
    int y;
    float deltaX;
    float deltaY;
    float angle;
};

class Map
{
public:
    int xSize;
    int ySize;
    int cellSize;
    Editor *editor;
    vector<int> mapMatrix;

    Map(int x, int y, int size, Editor *editor);

    void DrawMap2D(Color colors[]);
    void DrawRays(Player *player, Color colors[]);
    float Distance(float aX, float aY, float bX, float bY);
};