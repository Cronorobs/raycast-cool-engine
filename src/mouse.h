//Author: Roberto Abad Jiménez
//Date: 26/04/2020

class Mouse
{
public:
    int x;
    int y;
    bool lPressed;
    bool rPressed;

    void OnClick(int button, int state, int x, int y);
    void OnMove(int x, int y);

    Mouse();
};