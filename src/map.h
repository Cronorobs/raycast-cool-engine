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
        vector<Sprite> sprites;
        vector<int> mapMatrix;
        vector<int> eventMatrix;

        Map(int s, int cSize);

        void Resize();
        void DrawMap(Player *player);

    private:
        void DrawBackground();
};