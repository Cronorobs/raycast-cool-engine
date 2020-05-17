//Author: Roberto Abad Jiménez
//Date: 06/05/2020
class Map;
class Player;
#include <vector>
using namespace std;

class Sprite
{
    public:
        float x;
        float y;
        float distance;
        bool visible;

        Sprite(Map map, int cell);

        void Calculate(Player * p);
        void Draw(Player * p, int lineWidth, vector<float> zBuffer);

    private:
        int cellSize;
};