//Author: Roberto Abad Jiménez
//Date: 23/05/2020
#include "imgui/imgui.h"
#include <stdio.h>
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

static ImVec4 clear_color = ImVec4(.225f, .225f, .225f, 1.f);

//0 is editor, 1 is settings and 2 is 3D player
int state = 0;
int mapSize = 8;
bool tiles = true;
int currentTile = 0;
std::vector<int> map;

//Function declarations
void DrawMapTools();
void DrawMapEditor();
void DrawMapSettings();
void DrawEditorSettings();
void DrawGame();

void Display()
{
    switch (state)
    {
        case 0:
            DrawMapTools();
            DrawMapEditor();
            DrawMapSettings();
            break;
        case 1:
            //DrawEditorSettings();
            break;
        case 2:
            //DrawGame();
            break;
    }
}

void CreateMap(int size)
{
    map.clear();

    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - (size + 1) || i % size == 0 || i % size == size - 1) { map.push_back(1); }
        else { map.push_back(0); }
    }
}

int colorSize = 5;
ImColor colors [] = 
{
    ImColor(.0f, .0f, .0f),
    ImColor(.0f, .5f, .6f),
    ImColor(1.f, .0f, .5f),
    ImColor(.0f, .9f, .5f),
    ImColor(.9f, .9f, .4f)
};

ImColor Clear(ImColor base)
{
    return ImColor(base.Value.x + .2f, base.Value.y + .2f, base.Value.z + .2f, 1.f);
}

ImColor Dark(ImColor base)
{
    return ImColor(base.Value.x - .2f, base.Value.y - .2f, base.Value.z - .2f, 1.f);
}

void DrawMapTools()
{
    //Window Settings
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::SetNextWindowPos(ImVec2(24, 52), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(212, 512), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));

    ImGui::Begin("Map editor");
    if(tiles)
    {
        ImGui::Text("Tile Types:");
        for (int i = 0; i < colorSize; i++)
        {
            if(i > 0) { ImGui::SameLine(); }
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)colors[i]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)Clear(colors[i]));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)Dark(colors[i]));
            if(ImGui::Button("", ImVec2(32, 32))) { currentTile = i; }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }
    else
    {
        ImGui::Text("Event Types:");
        if(ImGui::Button("Character", ImVec2(96, 24))) { printf("Character"); }
        if(ImGui::Button("Player Start", ImVec2(96, 24))) { printf("Player"); }
    }
    
    ImGui::Text("Mode");
    if(ImGui::Button("Tiles", ImVec2(88, 24))) { tiles = true; }
    ImGui::SameLine();
    if(ImGui::Button("Events", ImVec2(88, 24))) { tiles = false; }
    if(ImGui::Button("Change Map size", ImVec2(128, 24))) 
    {
        if (mapSize == 8) { mapSize = 16; }
        else if (mapSize == 16) { mapSize = 32; }
        else if (mapSize == 32) { mapSize = 8; }
        CreateMap(mapSize);
    }
    ImGui::End();
}

void DrawMapEditor()
{
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::SetNextWindowPos(ImVec2(276, 52), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(526, 552), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));
    ImGui::Begin("Map");

    int aux = 0;
    for (int i = 0; i < mapSize * mapSize; i++)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
        if (aux % mapSize + 1 != 1) { ImGui::SameLine(); }
        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)colors[map[i]]);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)Clear(colors[map[i]]));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)Dark(colors[map[i]]));
        if(ImGui::Button("", ImVec2(512/mapSize - 2, 512/mapSize - 2))) { map[i] = currentTile; }
        ImGui::PopID();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        aux++;
    }

    ImGui::End();
}

void DrawMapSettings()
{
    //Settings menu
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::SetNextWindowPos(ImVec2(844, 52), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(212, 512), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));
    ImGui::Begin("Settings");
    ImGui::Text("Character Text");
    static char str[256] = "";
    ImGui::InputTextMultiline("", str, 256, ImVec2(184, 76));
    if(ImGui::Button("Save")) { printf(str); }
    ImGui::End();
}

void my_display_code()
{
    /* // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    } */

    // 3. Show another simple window.
    /* if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    } */
}

void glut_display_func()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    Display();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

int main(int argc, char** argv)
{
    // Create GLUT window
    glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(1080, 640);
    glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example");

    // Setup GLUT display function
    // We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
    // otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
    glutDisplayFunc(glut_display_func);

    map = std::vector<int>();
    CreateMap(mapSize);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();


    io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("../fonts/Roboto-Medium.ttf", 16.0f);
    io.FontDefault = font;

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}