//Author: Roberto Abad Jiménez
//Date: 26/04/2020
#include <GL/glut.h>
#include <string>
#include <vector>
class Mouse;
struct Color;
class Button;
using namespace std;

class Editor
{
public:
    int width;
    int state;
    int height;
    int mapButtons;
    int currentTile;
    int currentButton;
    Mouse *mouse;
    string theString;
    bool events;
    bool displayingText;
    vector<Button *> buttons;

    void DrawBackground();
    void DrawEditor(int colorSize);
    void UpdateString(char key);
    void DrawBox(int x, int y, int w, int h, Color c);
    void DrawBox(int x, int y, int w, int h, Color c, float t);
    void DrawText(string text, int x, int y, Color c);
    void DrawText(string text, int x, int y);
    void DrawButtons(int from, int to);
    void DrawUI();
    void CheckButtons(int from, int to);
    void CheckHighlighted(int from, int to);

    Editor(Mouse *mouse, int width, int height);

private:
    void *font;
};