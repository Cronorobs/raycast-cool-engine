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

//Objects
Mouse mouse = Mouse();
Editor editor = Editor(&mouse, 1080, 640);
Player player = {0, 0, 0, 0, 0};
Map map = Map(8, 8, 64, &editor);

//Function declarations
void Display();
void CreateButtons();
void ButtonAFunction();
void ButtonBFunction();
void ButtonCFunction();
void ChangeCell();
void OnMove(int x, int y);
void OnClick(int button, int state, int x, int y);

Color colors[4] =
    {
        {.0f, .0f, .0f},
        {.0f, .5f, .6f},
        {1.f, .0f, .5f},
        {.0f, .9f, .5f}};

void CreateButtons()
{
    Color darkGrey = {.4f, .4f, .4f};

    editor.buttons.push_back(new Button(340, 20, 88, 32, 0, "Editor", darkGrey, ButtonAFunction));
    editor.buttons.push_back(new Button(484, 20, 88, 32, 0, "2D View", darkGrey, ButtonBFunction));
    editor.buttons.push_back(new Button(624, 20, 88, 32, 0, "3D View", darkGrey, ButtonCFunction));

    int x = editor.width / 4 + 1;
    int y = 65;
    int aux = 0;

    for (int i = 0; i < 64; i++)
    {
        editor.buttons.push_back(new Button(x, y, 62, 62, i, "", colors[map.mapMatrix[i]], ChangeCell));

        if (aux == 7)
        {
            y += map.cellSize;
            x = editor.width / 4 + 1;
            aux = 0;
        }
        else
        {
            x += map.cellSize;
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

void ChangeCell()
{
    printf("Original for cell %i: %i  ", editor.currentButton, map.mapMatrix[editor.currentButton]);
    map.mapMatrix[editor.currentButton] = (map.mapMatrix[editor.currentButton] + 1) % 4;
    editor.buttons[editor.currentButton + 3]->color = colors[map.mapMatrix[editor.currentButton]];
    printf("Changed to: %i\n", map.mapMatrix[editor.currentButton]);
    Display();
}

//OpenGL functions
void OnMove(int x, int y)
{
    mouse.OnMove(x, y);
    if (editor.state == 0)
    {
        editor.CheckHighlighted(0, 67);
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
            editor.CheckButtons(0, 67);
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
    editor.DrawEditor();
    map.DrawMap2D(colors);
    editor.DrawBackground();
    drawPlayer();
    map.DrawRays(&player, colors);
    glutSwapBuffers();
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
            player.angle -= .1f;
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
            player.angle += .1f;
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
    glutMainLoop();
}