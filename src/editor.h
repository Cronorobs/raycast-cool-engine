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
    int state;
    int currentButton;
    int width;
    int height;
    Mouse *mouse;
    vector<Button *> buttons;

    void DrawBackground();
    void DrawEditor();
    void DrawBox(int x, int y, int w, int h, Color c);
    void DrawText(string text, int x, int y);
    void DrawButtons(int from, int to);
    void CheckButtons(int from, int to);
    void CheckHighlighted(int from, int to);

    Editor(Mouse *mouse, int width, int height);

private:
    void *font;
};