#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <iostream>
#include <vector>
class Map;
class Player;

class Editor2
{
    public:
        int state;
        int currentTile;
        bool tiles;

        Editor2();

        void Display(Map * map, Player * player);
        void DrawEditorSettings();
        void DrawGame();

    private:
        std::vector<ImColor> colors;

        void DrawMenuBar();
        void DrawMapTools(Map * map);
        void DrawMapEditor(Map * map);
        void DrawMapSettings();
        ImColor Clear(ImColor base);
        ImColor Dark(ImColor base);
};