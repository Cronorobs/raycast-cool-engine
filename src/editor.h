#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <iostream>
#include <vector>
class Map;
class Player;

class Editor
{
    public:
        int state;
        bool tiles;
        int currentTile;
        std::vector<ImColor> colors;

        Editor();

        void Display(Map * map, Player * player);
        void DrawEditorSettings();
        void DrawGame();

    private:

        void DrawMenuBar();
        void DrawMapTools(Map * map);
        void DrawMapEditor(Map * map);
        void DrawMapSettings();
        ImColor Clear(ImColor base);
        ImColor Dark(ImColor base);
};