//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include <vector>
using namespace std;
class Editor;
class Player;
class Sprite;
struct Color;

class Map
{
    public:
        int size;
        int cellSize;
        Editor *editor;
        vector<Sprite> sprites;
        vector<int> mapMatrix;

        Map(int s, int cSize, Editor *editor);

        void Resize(int newSize);
        void DrawMap2D(Color colors[]);
        void DrawRays(Player *player, Color colors[], int length);
};