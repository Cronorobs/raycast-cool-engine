//Author: Roberto Abad Jiménez
//Date: 26/04/2020
#include <string>
using namespace std;
typedef void (*ButtonCallback)();
class Editor;

struct Color
{
public:
    float r;
    float g;
    float b;
};

class Button
{
public:
    int id;
    bool highlighted;
    Color color;

    ButtonCallback callback;

    void DrawButton(Editor editor);
    bool IsMouseOver(int mouseX, int mouseY);

    Button(int x, int y, int w, int h, int id, string text, Color color, ButtonCallback callback);
    Button(int x, int y, int w, int h, int id, string text, Color color, ButtonCallback callback, bool transp);

private:
    int x;
    int y;
    int w;
    int h;
    bool transparent;
    string text;
};