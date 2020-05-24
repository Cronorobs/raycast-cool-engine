//Author: Roberto Abad Jiménez
//Date: 27/04/2020
#include <vector>
struct ImColor;
class Player;
class Sprite;

class Map
{
    public:
        int size;
        int cellSize;
        std::vector<Sprite> sprites;
        std::vector<int> mapMatrix;
        std::vector<int> eventMatrix;

        Map(int s, int cSize);

        void Resize();
        void DrawMap(Player *player, std::vector<ImColor> colors);

    private:
        void DrawBackground();
};