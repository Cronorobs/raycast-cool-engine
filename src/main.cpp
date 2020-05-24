//Author: Roberto Abad Jiménez
//Date: 24/04/2020
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include "player.h"
#include "editor.h"
#include "map.h"
#include "sprite.h"

#define PI 3.1415926535
#define DR 0.0174533

//Objects
Editor edi = Editor();
Player player = Player(256, 256);
Map map = Map(8, 64);

void Display()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    edi.Display(&map, &player);

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(.25f, .25f, .25f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(edi.state == 2) { map.DrawMap(&player, edi.colors); }
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

void Resize(int width, int height)
{
    glutReshapeWindow(1080, 640);
}

void Input(unsigned char key, int x, int y)
{
    if (edi.state == 2)
    {
        switch (key)
        {
        case 'w':
            player.x += player.deltaX;
            player.y += player.deltaY;
            break;
        case 'a':
            player.angle -= 5 * DR;
            if (player.angle < 0) { player.angle += 2 * PI; }
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        case 's':
            player.x -= player.deltaX;
            player.y -= player.deltaY;
            break;
        case 'd':
            player.angle += 5 * DR;
            if (player.angle > 2 * PI) { player.angle -= 2 * PI; }
            player.deltaX = cos(player.angle) * 5;
            player.deltaY = sin(player.angle) * 5;
            break;
        }
        glutPostRedisplay();
    }
}

void Initialize()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    //Setup Font
    io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("../fonts/Roboto-Medium.ttf", 16.0f);
    io.FontDefault = font;

    //GLUT Config
    glClearColor(.225f, .225f, .225f, 1.f);
    gluOrtho2D(0, 1080, 640, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1080, 640);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 1080) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 640) / 2);
    glutCreateWindow("Raycast Cool Engine");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Input);

    Initialize();
    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}