#pragma once
#include "Renderer.hpp"
#include "Config.hpp"
#include "IconsFont.h"
#include "IconsFontAwesome.h"

void OnInitializeStyle()
{
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF(RobotoFont, sizeof(RobotoFont), 12, NULL, io.Fonts->GetGlyphRangesCyrillic());

    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryTTF(IconsFont, sizeof(IconsFont), 18.f, &icons_config, icon_ranges);

    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(8.f, 8.f);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(6.f, 6.f);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(6, 6);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;
    style->FrameBorderSize = 0.f;
    style->ChildBorderSize = 0.f;
    style->WindowBorderSize = 0.f;
    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

    style->Colors[ImGuiCol_Text] = Booster::Main::Colors::Text.ToImColor();
    style->Colors[ImGuiCol_WindowBg] = Booster::Main::Colors::Background.ToImColor();
    style->Colors[ImGuiCol_FrameBg] = Booster::Main::Colors::Secondary.ToImColor();
    style->Colors[ImGuiCol_FrameBgHovered] = Booster::Main::Colors::Secondary.ToImColor();
    style->Colors[ImGuiCol_FrameBgActive] = Booster::Main::Colors::Secondary.ToImColor();
    style->Colors[ImGuiCol_TitleBg] = Booster::Main::Colors::Primary.ToImColor();
    style->Colors[ImGuiCol_TitleBgActive] = Booster::Main::Colors::Primary.ToImColor(true);
    style->Colors[ImGuiCol_TitleBgCollapsed] = Booster::Main::Colors::Primary.ToImColor(true);
    style->Colors[ImGuiCol_CheckMark] = Booster::Main::Colors::Primary.ToImColor();
    style->Colors[ImGuiCol_Separator] = Booster::Main::Colors::Background.ToImColor();
    style->Colors[ImGuiCol_SeparatorActive] = Booster::Main::Colors::Background.ToImColor();
    style->Colors[ImGuiCol_SeparatorHovered] = Booster::Main::Colors::Background.ToImColor();
    style->Colors[ImGuiCol_Button] = Booster::Main::Colors::Primary.ToImColor();
    style->Colors[ImGuiCol_ButtonActive] = Booster::Main::Colors::Primary.ToImColor(true);
    style->Colors[ImGuiCol_ButtonHovered] = Booster::Main::Colors::Primary.ToImColor(true);
    style->Colors[ImGuiCol_Header] = Booster::Main::Colors::Primary.ToImColor();
    style->Colors[ImGuiCol_HeaderActive] = Booster::Main::Colors::Primary.ToImColor(true);
    style->Colors[ImGuiCol_HeaderHovered] = Booster::Main::Colors::Primary.ToImColor(true);
}

bool Binding = false;
int BindingID = 0;

int const KeyNames[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,
    VK_MULTIPLY,
    VK_ADD,
    VK_SEPARATOR,
    VK_SUBTRACT,
    VK_DECIMAL,
    VK_DIVIDE,
    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
};

const char* GetKeyText(int key)
{
    LPARAM lParam;

    UINT sc = MapVirtualKey(key, 0);
    lParam = sc << 16;

    char buf[256];
    GetKeyNameTextA(lParam, buf, 256);
    return buf;
}

void KeyBind(int& key, int width, int id = 0)
{
    bool currentBinding = (Binding && BindingID == id);
    if (ImGui::Button(currentBinding ? "..." : GetKeyText(key), ImVec2(width, 30))) {
        Binding = !Binding;
        BindingID = id;
    }
    if (GetKeyState(VK_ESCAPE) & 0x8000) {
        Binding = false;
        return;
    }
    if (currentBinding) {
        for (int k : KeyNames)
        {
            if (GetKeyState(k) & 0x8000) {
                Binding = false;
                key = k;
            }
        }
    }
}

void OnRenderState()
{
    /*
    Booster::Config::Save();
    Если необходимо добавить сохранение конфига используйте https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
    json j;
    j["GrassDisabled"] = Config::GrassDiabled;
    j.dump();
    и потом в файлик
    а для загрузки выносим с файлика допустим в std::string или const char*
    потом json j = json::parse(str);
    Config::GrassDisabled = j["GrassDisabled"].get<bool>();
    */
}

int activeTab = -1;

void OnRender()
{
    float WindowHeight = 110;

    switch (activeTab) {
        case 0:
            WindowHeight = 135;
            break;
        case 1:
            WindowHeight = 250;
            break;
        case 2:
            WindowHeight = 130;
            break;
    }

    ImGui::SetNextWindowSize(ImVec2(250, WindowHeight));
    ImGui::Begin(Booster::Main::Title, nullptr, ImGuiConfigFlags_NoMouseCursorChange | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (activeTab == -1) {
        ImGui::PushStyleColor(ImGuiCol_Button, Booster::Main::Colors::Secondary.ToImColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Booster::Main::Colors::Secondary.ToImColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Booster::Main::Colors::Secondary.ToImColor());
        ImGui::Columns(3);

        float column_width = ImGui::GetContentRegionAvailWidth();

        if (ImGui::Button(ICON_FA_BOLT, ImVec2(column_width, column_width))) activeTab = 0;
        ImGui::NextColumn();
        if (ImGui::Button(ICON_FA_KEYBOARD, ImVec2(column_width, column_width))) activeTab = 1;
        ImGui::NextColumn();
        if (ImGui::Button(ICON_FA_QUESTION_CIRCLE, ImVec2(column_width, column_width))) activeTab = 2;

        ImGui::EndColumns();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    if (activeTab >= 0) {
        ImGui::PushStyleColor(ImGuiCol_Button, Booster::Main::Colors::Secondary.ToImColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Booster::Main::Colors::Secondary.ToImColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Booster::Main::Colors::Secondary.ToImColor());
        if (ImGui::Button(ICON_FA_CHEVRON_LEFT, ImVec2(32, 32))) activeTab = -1;
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    if (activeTab == 0) {
        ImGui::Checkbox(u8"Выключить тени", &Booster::Config::DisableShadows);
        ImGui::Checkbox(u8"Выключить траву", &Booster::Config::DisableGrass);
    }

    if (activeTab == 1) {
        for (int i = 0; i < Booster::Main::Binds.size(); i++) {
            if (ImGui::CollapsingHeader(Booster::Main::Binds[i].text)) {
                ImGui::Columns(2);
                ImGui::Checkbox(u8"Включен##AA", &Booster::Main::Binds[i].enabled);
                ImGui::NextColumn();
                if (!Booster::Main::Binds[i].enabled) {
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                }
                KeyBind(Booster::Main::Binds[i].key, ImGui::GetContentRegionAvailWidth(), 0);
                if (!Booster::Main::Binds[i].enabled) {
                    ImGui::PopItemFlag();
                    ImGui::PopStyleVar();
                }
                ImGui::EndColumns();
            }
        }
    }

    if (activeTab == 2) {
        ImGui::Text(u8"Разработчик: azp2033");
        ImGui::PushStyleColor(ImGuiCol_Text, Color(114, 145, 247).ToImColor());
        ImGui::Text(u8"vk.com/azp2033");
        ImGui::Text(u8"github.com/azp2033");
        ImGui::PopStyleColor();
    }

    ImGui::End();
}