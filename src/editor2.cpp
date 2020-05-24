#include "editor2.h"
#include "player.h"
#include "map.h"

Editor2::Editor2()
{
    state = 0;
    currentTile = 0;
    tiles = true;

    colors = std::vector<ImColor>();
    colors.push_back(ImColor(.0f, .0f, .0f));
    colors.push_back(ImColor(.0f, .5f, .6f));
    colors.push_back(ImColor(1.f, .0f, .5f));
    colors.push_back(ImColor(.0f, .9f, .5f));
    colors.push_back(ImColor(.9f, .9f, .4f));
}

void Editor2::Display(Map * map, Player * player)
{
    DrawMenuBar();

    switch (state)
    {
        case 0:
            DrawMapTools(map);
            DrawMapEditor(map);
            DrawMapSettings();
            break;
        case 1:
            //DrawEditorSettings();
            break;
        case 2:
            DrawGame();
            break;
    }
}

void Editor2::DrawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Map Editor"))
        {
            if (ImGui::MenuItem("Tile Editor")) { state = 0; tiles = true; }
            if (ImGui::MenuItem("Event Editor")) { state = 0; tiles = false; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::MenuItem("Open")) { state = 1; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Game View"))
        {
            if (ImGui::MenuItem("Open")) { state = 2; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Editor2::DrawMapTools(Map * map)
{
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::SetNextWindowPos(ImVec2(24, 52), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(212, 512), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));

    ImGui::Begin("Map editor");
    if(ImGui::Button("3D view")) { state = 2; }
    if(tiles)
    {
        ImGui::Text("Tile Types:");
        for (int i = 0; i < colors.size(); i++)
        {
            if(i != 0 && i % 2 != 0) { ImGui::SameLine(); }
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)colors[i]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)Clear(colors[i]));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)Dark(colors[i]));
            char c[16] = "Floor";
            if(i > 0) { sprintf(c, "Wall % i", i); }
            if(ImGui::Button(c, ImVec2(80, 32))) { currentTile = i; }
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }
    else
    {
        ImGui::Text("Event Types:");
        ImGui::Button("Character", ImVec2(96, 24));//) { printf("Character"); }
        ImGui::Button("Player Start", ImVec2(96, 24));//) { printf("Player"); }
    }
    
    ImGui::Text("Mode");
    if(ImGui::Button("Tiles", ImVec2(88, 24))) { tiles = true; }
    ImGui::SameLine();
    if(ImGui::Button("Events", ImVec2(88, 24))) { tiles = false; }
    if(ImGui::Button("Change Map size", ImVec2(128, 24))) 
    {
        if (map->size == 8) { map->size = 16; }
        else if (map->size == 16) { map->size = 32; }
        else if (map->size == 32) { map->size = 8; }
        map->Resize();
    }
    ImGui::End();
}

void Editor2::DrawMapEditor(Map * map)
{
    ImGui::SetNextWindowBgAlpha(.5f);
    ImGui::SetNextWindowPos(ImVec2(276, 52), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(526, 552), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));
    ImGui::Begin("Map");

    int aux = 0;
    for (int i = 0; i < map->size * map->size; i++)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
        if (aux % map->size + 1 != 1) { ImGui::SameLine(); }
        ImGui::PushID(i);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)colors[map->mapMatrix[i]]);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)Clear(colors[map->mapMatrix[i]]));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)Dark(colors[map->mapMatrix[i]]));
        if(ImGui::Button("", ImVec2(512/map->size - 2, 512/map->size - 2))) { map->mapMatrix[i] = currentTile; }
        ImGui::PopID();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        aux++;
    }

    ImGui::End();
}

void Editor2::DrawMapSettings()
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
    ImGui::Button("Save"); //) { printf(str); }
    ImGui::End();
}

void Editor2::DrawEditorSettings()
{

}

void Editor2::DrawGame()
{
    //Draw a window for the game
    ImGui::SetNextWindowBgAlpha(.0f);
    ImGui::SetNextWindowPos(ImVec2(220, 80));
    ImGui::SetNextWindowSize(ImVec2(640, 480));
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1),    ImVec2(-1, FLT_MAX));
    ImGui::Begin("Game view");
    //ImGui::Button("Save"); //) { printf(str); }
    ImGui::End();
}

ImColor Editor2::Clear(ImColor base)
{
    return ImColor(base.Value.x + .2f, base.Value.y + .2f, base.Value.z + .2f, 1.f);
}

ImColor Editor2::Dark(ImColor base)
{
    return ImColor(base.Value.x - .2f, base.Value.y - .2f, base.Value.z - .2f, 1.f);
}