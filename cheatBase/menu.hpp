#pragma once
#include "imgui/imgui.h"
#include <string>
#include <vector>

#define MENULIST_ITEMS_AMOUNT_CAP 20;
#define MENULIST_DRAW_POS_X 350;
#define MENULIST_DRAW_POS_Y 350;

class MenuList;
class MenuItem
{
public:
    MenuItem(const char* label) : label(label) {};
    virtual std::string getLabel() const { return label; };  // Marked as const

    virtual std::string getString() const { return getLabel(); };  // Marked as const
    virtual void processKeyInput() {};

private:
    const char* label;
};

class MenuItemGoto : public MenuItem
{
public:
    MenuItemGoto(const char* label, MenuList** pCurMenu, int* pCurItem, MenuList* pMenu)
        : MenuItem(label), pMenu(pMenu), pCurItem(pCurItem), pCurMenu(pCurMenu) {};

    std::string getString() const override { return getLabel(); };  // Marked as const
    void processKeyInput() override {
        if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            *pCurItem = 0;
            *pCurMenu = pMenu;
        }
    };
private:
    int* pCurItem;
    MenuList** pCurMenu;
    MenuList* pMenu;
};

class MenuItemBool : public MenuItem
{
public:
    MenuItemBool(const char* label, bool* pBool) : MenuItem(label), pBool(pBool) {};
    std::string getString() const override { return getLabel() + (*pBool ? ": ON" : ": OFF"); };  // Marked as const
    void processKeyInput() override {
        if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            *pBool = !*pBool;
        }
    };
private:
    bool* pBool;
};

class MenuItemInt : public MenuItem
{
public:
    MenuItemInt(const char* label, int* pInt)
        : MenuItem(label), pInt(pInt), maxVal(INT_MAX), minVal(INT_MIN), step(1) {};
    MenuItemInt(const char* label, int* pInt, const int step)
        : MenuItem(label), pInt(pInt), maxVal(INT_MAX), minVal(INT_MIN), step(step) {};
    MenuItemInt(const char* label, int* pInt, const int step, const int minVal, const int maxVal)
        : MenuItem(label), pInt(pInt), maxVal(maxVal), minVal(minVal), step(step) {};

    std::string getString() const override { return getLabel() + ": " + std::to_string(*pInt); };  // Marked as const
    void processKeyInput() override {
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            *pInt = max(minVal, (*pInt) - step);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            *pInt = min(maxVal, (*pInt) + step);
        }
    };
private:
    int* pInt;
    const int maxVal;
    const int minVal;
    const int step;
};

class MenuItemFloat : public MenuItem
{
public:
    MenuItemFloat(const char* label, float* pFloat)
        : MenuItem(label), pFloat(pFloat), maxVal(FLT_MAX), minVal(-FLT_MAX), step(1) {};
    MenuItemFloat(const char* label, float* pFloat, const float step)
        : MenuItem(label), pFloat(pFloat), maxVal(FLT_MAX), minVal(-FLT_MAX), step(step) {};
    MenuItemFloat(const char* label, float* pFloat, const float step, const float minVal, const float maxVal)
        : MenuItem(label), pFloat(pFloat), maxVal(maxVal), minVal(minVal), step(step) {};

    std::string getString() const override { return getLabel() + ": " + std::to_string(*pFloat); };  // Marked as const
    void processKeyInput() override {
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            *pFloat = max(minVal, (*pFloat) - step);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            *pFloat = min(maxVal, (*pFloat) + step);
        }
    };
private:
    float* pFloat;
    const float maxVal;
    const float minVal;
    const float step;
};

class MenuItemType : public MenuItem
{
public:
    MenuItemType(const char* label, int* pType, const char* types[], const int maxVal)
        : MenuItem(label), pType(pType), types(types), maxVal(maxVal) {};

    std::string getString() const override { return getLabel() + ": " + std::string(types[*pType]); };  // Marked as const
    void processKeyInput() override {
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            *pType = max(0, (*pType) - 1);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            *pType = min(maxVal, (*pType) + 1);
        }
    };
private:
    int* pType;
    const char** types;
    const int maxVal;
};


class MenuList
{
public:
    MenuList(int* pCurItem) : curItem(pCurItem) {};

    bool appendItem(MenuItem* item) {
        if (optionsList.size() < maxItemsAmount) {
            optionsList.push_back(item);
            return true;
        }
        return false;
    }

    void draw() {
        const int x = MENULIST_DRAW_POS_X; const int y = MENULIST_DRAW_POS_Y;
        ImVec2 pos = ImVec2(x, y);
        ImVec4 selectedColor = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
        ImVec4 normalColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        for (int i = 0; i < optionsList.size(); i++) {
            const MenuItem* item = optionsList[i];
            if (i == *curItem) {
                ImGui::GetForegroundDrawList()->AddText(pos, ImColor(selectedColor), ("> " + item->getString()).c_str());
            }
            else {
                ImGui::GetForegroundDrawList()->AddText(pos, ImColor(normalColor), item->getString().c_str());
            }
            pos.y += 20;
        }
    }

    void processInput() {
        optionsList[*curItem]->processKeyInput();
    }

    int getItemsAmount() {
        return optionsList.size();
    }

private:
    const int maxItemsAmount = MENULIST_ITEMS_AMOUNT_CAP;
    int* curItem;
    std::vector<MenuItem*> optionsList;
};