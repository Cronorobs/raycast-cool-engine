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
    int size;
    int cellSize;
    Editor *editor;
    vector<int> mapMatrix;

    Map(int s, int cSize, Editor *editor);

    void Resize(int newSize);
    void DrawMap2D(Color colors[]);
    void DrawRays(Player *player, Color colors[], int length);
    float Distance(float aX, float aY, float bX, float bY);
};