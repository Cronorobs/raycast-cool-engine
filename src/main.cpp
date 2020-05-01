//Author: Roberto Abad Jiménez
//Date: 24/04/2020
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include "mouse.h"
#include "editor.h"
#include "button.h"
#include "map.h"

#define PI 3.1415926535
#define DR 0.0174533

//Objects
Mouse mouse = Mouse();
Editor editor = Editor(&mouse, 1080, 640);
Player player = {0, 0, 0, 0, 0};
Map map = Map(8, 64, &editor);

//Function declarations
void Display();
void CreateButtons();
void ButtonAFunction();
void ButtonBFunction();
void ButtonCFunction();
void ChangeCell();
void ChangeMapSize();
void TileFunction();
void OnMove(int x, int y);
void OnClick(int button, int state, int x, int y);

const int colorSize = 5;
Color colors[colorSize] =
    {
        {.0f, .0f, .0f},
        {.0f, .5f, .6f},
        {1.f, .0f, .5f},
        {.0f, .9f, .5f},
        {.9f, .9f, .4f}};

void CreateButtons()
{
    Color darkGrey = {.4f, .4f, .4f};

    //Editor section buttons
    editor.buttons.push_back(new Button(340, 20, 88, 32, 0, "Editor", darkGrey, ButtonAFunction));
    editor.buttons.push_back(new Button(484, 20, 88, 32, 0, "2D View", darkGrey, ButtonBFunction));
    editor.buttons.push_back(new Button(624, 20, 88, 32, 0, "3D View", darkGrey, ButtonCFunction));

    //Change map size
    editor.buttons.push_back(new Button(72, 400, 88, 52, 0, "Change\'map size", darkGrey, ChangeMapSize));

    //Walls and ground editor buttons
    int y = 96;
    for (int i = 0; i < colorSize; i++)
    {
        editor.buttons.push_back(new Button(72, y, 96, 32, i, "", colors[i], TileFunction));
        y += 48;
    }

    int x = editor.width / 4 + 1;
    y = 65;
    int aux = 0;
    float buttonSize = 512 / map.size;

    for (int i = 0; i < map.size * map.size; i++)
    {
        editor.buttons.push_back(new Button(x, y, buttonSize - 2, buttonSize - 2, i, "", colors[map.mapMatrix[i]], ChangeCell));

        if (aux == map.size - 1)
        {
            y += buttonSize;
            x = editor.width / 4 + 1;
            aux = 0;
        }
        else
        {
            x += buttonSize;
            aux++;
        }
    }
}

//Callbacks
void ButtonAFunction()
{
    editor.state = 0;
    Display();
}
void ButtonBFunction()
{
    editor.state = 1;
    Display();
}
void ButtonCFunction()
{
    editor.state = 2;
    Display();
}

void ChangeMapSize()
{
    //Delete previous editor buttons
    for (int i = 0; i < map.size * map.size; i++)
    {
        editor.buttons.pop_back();
    }

    //Change the map object's size
    if (map.size == 8)
    {
        map.Resize(16);
    }
    else if (map.size == 16)
    {
        map.Resize(32);
    }
    else if (map.size == 32)
    {
        map.Resize(8);
    }

    //Create new buttons
    int x = editor.width / 4 + 1;
    int y = 65;
    int aux = 0;
    float buttonSize = 512 / map.size;

    for (int i = 0; i < map.size * map.size; i++)
    {
        editor.buttons.push_back(new Button(x, y, buttonSize - 2, buttonSize - 2, i, "", colors[map.mapMatrix[i]], ChangeCell));

        if (aux == map.size - 1)
        {
            y += buttonSize;
            x = editor.width / 4 + 1;
            aux = 0;
        }
        else
        {
            x += buttonSize;
            aux++;
        }
    }
}

void ChangeCell()
{
    map.mapMatrix[editor.currentButton] = editor.currentTile;
    editor.buttons[editor.currentButton + 4 + colorSize]->color = colors[editor.currentTile];
    Display();
}

void TileFunction()
{
    editor.currentTile = editor.currentButton;
}

//OpenGL functions
void OnMove(int x, int y)
{
    mouse.OnMove(x, y);
    if (editor.state == 0)
    {
        editor.CheckHighlighted(0, editor.buttons.size());
    }
    else
    {
        editor.CheckHighlighted(0, 3);
    }
    Display();
}

void OnClick(int button, int state, int x, int y)
{
    mouse.OnClick(button, state, x, y);
    if (mouse.lPressed)
    {
        if (editor.state == 0)
        {
            editor.CheckButtons(0, editor.buttons.size());
        }
        else
        {
            editor.CheckButtons(0, 3);
        }
    }
}

void drawPlayer()
{
    if (editor.state == 1)
    {
        glColor3f(0.f, 1.f, 1.f);
        glPointSize(8.f);
        glBegin(GL_POINTS);
        glVertex2i(player.x + editor.width / 4, player.y + 64);
        glEnd();

        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(player.x + editor.width / 4, player.y + 64);
        glVertex2i(player.x + player.deltaX * 3 + editor.width / 4, player.y + player.deltaY * 3 + 64);
        glEnd();
    }
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    editor.DrawEditor(colorSize);
    map.DrawMap2D(colors);
    editor.DrawBackground();
    drawPlayer();
    map.DrawRays(&player, colors, colorSize);
    glutSwapBuffers();
}

void Resize(int width, int height)
{
    glutReshapeWindow(editor.width, editor.height);
}

void Input(unsigned char key, int x, int y)
{
    if (editor.state > 0)
    {
        switch (key)
        {
        case 'w':
            player.x += player.deltaX;
            player.y += player.deltaY;
            break;
        case 'a':
            player.angle -= 5 * DR;
            if (player.angle < 0)
            {
                player.angle += 2 * PI;
            }
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        case 's':
            player.x -= player.deltaX;
            player.y -= player.deltaY;
            break;
        case 'd':
            player.angle += 5 * DR;
            if (player.angle > 2 * PI)
            {
                player.angle -= 2 * PI;
            }
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        }
        glutPostRedisplay();
    }
}

void Initialize()
{
    glClearColor(.25f, .25f, .25f, 1.f);
    gluOrtho2D(0, editor.width, editor.height, 0);
    player.x = 256;
    player.y = 256;
    player.deltaX = cos(player.angle) * 5;
    player.deltaY = sin(player.angle) * 5;
    CreateButtons();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(editor.width, editor.height);
    glutCreateWindow("Raycast Cool Engine");
    Initialize();
    glutDisplayFunc(Display);
    glutMouseFunc(OnClick);
    glutPassiveMotionFunc(OnMove);
    glutKeyboardFunc(Input);
    glutReshapeFunc(Resize);
    glutMainLoop();
}