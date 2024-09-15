#include "menu.hpp"

namespace Nav
{
    int curItem = 0;

    MenuList* mainMenu = new MenuList(&curItem);
    MenuList* aimMenu = new MenuList(&curItem);
    MenuList* espMenu = new MenuList(&curItem);
    MenuList* movMenu = new MenuList(&curItem);
    MenuList* miscMenu = new MenuList(&curItem);

    MenuList* curMenu = mainMenu;

    void init() {
        ///////////////////////////////////////////////////////////////
        /*                      [MAIN MENU]                          */
        ///////////////////////////////////////////////////////////////
        mainMenu->appendItem(new MenuItemGoto("AIMBOT", &curMenu, &curItem, aimMenu));
        mainMenu->appendItem(new MenuItemGoto("ESP", &curMenu, &curItem, espMenu));
        mainMenu->appendItem(new MenuItemGoto("MOVEMENT", &curMenu, &curItem, movMenu));
        mainMenu->appendItem(new MenuItemGoto("MISC", &curMenu, &curItem, miscMenu));

        ///////////////////////////////////////////////////////////////
        /*                      [AIMBOT MENU]                        */
        ///////////////////////////////////////////////////////////////
        aimMenu->appendItem(new MenuItemGoto("GO BACK", &curMenu, &curItem, mainMenu));
        aimMenu->appendItem(new MenuItemBool("ENABLED", &settings::aimbot::bEnabled));
        aimMenu->appendItem(new MenuItemBool("TEAM CHECK", &settings::aimbot::bIgnoreTeam));
        aimMenu->appendItem(new MenuItemBool("VISIBLE CHECK", &settings::aimbot::bOnlyVisible));
        aimMenu->appendItem(new MenuItemBool("AUTOFIRE", &settings::aimbot::bAutoFire));
        aimMenu->appendItem(new MenuItemType("PRIORITY TYPE", &settings::aimbot::aimType, settings::aimbot::workTypeString, 1));
        aimMenu->appendItem(new MenuItemFloat("FOV", &settings::aimbot::maxFov, 0.3f, 0, 360));
        aimMenu->appendItem(new MenuItemInt("SELF PREDICT", &settings::aimbot::selfPredict, 5));
        aimMenu->appendItem(new MenuItemInt("TARGET PREDICT", &settings::aimbot::targetPredict, 5));

        ///////////////////////////////////////////////////////////////
        /*                      [ESP MENU]                           */
        ///////////////////////////////////////////////////////////////
        espMenu->appendItem(new MenuItemGoto("GO BACK", &curMenu, &curItem, mainMenu));
        espMenu->appendItem(new MenuItemBool("ENABLED", &settings::esp::bEnabled));
        espMenu->appendItem(new MenuItemBool("DRAW TEAM", &settings::esp::bDrawTeam));
        espMenu->appendItem(new MenuItemBool("DRAW HEAD", &settings::esp::bDrawHead));
        espMenu->appendItem(new MenuItemBool("DRAW NAME", &settings::esp::bDrawName));

        ///////////////////////////////////////////////////////////////
        /*                      [MOVEMENT MENU]                      */
        ///////////////////////////////////////////////////////////////
        movMenu->appendItem(new MenuItemGoto("GO BACK", &curMenu, &curItem, mainMenu));
        movMenu->appendItem(new MenuItemBool("BUNNYHOP", &settings::movement::bBhopEnabled));
        movMenu->appendItem(new MenuItemBool("NO ACCELERATION", &settings::movement::bNoAcceleration));
        movMenu->appendItem(new MenuItemFloat("VELOCITY HACK", &settings::movement::velHack, 0.1f, 0.5f, 20.0f));
        movMenu->appendItem(new MenuItemBool("BRAKE JUMP ANIMS", &settings::movement::bBrakeAnims));


        ///////////////////////////////////////////////////////////////
        /*                      [MISC MENU]                          */
        ///////////////////////////////////////////////////////////////
        miscMenu->appendItem(new MenuItemGoto("GO BACK", &curMenu, &curItem, mainMenu));
        miscMenu->appendItem(new MenuItemBool("NO GUN WAIT", &settings::misc::bNoGunWait));
        miscMenu->appendItem(new MenuItemBool("NO RECOIL", &settings::misc::bNoRecoil));
        miscMenu->appendItem(new MenuItemBool("INFINITE AMMO", &settings::misc::bInfiniteAmmo));
        miscMenu->appendItem(new MenuItemBool("INFINITE HP", &settings::misc::bInfiniteHealth));
    }

    void drawMenu() {
        curMenu->draw();
    }

    void processInput() {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            curItem = max(0, curItem - 1);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            curItem = min((curMenu->getItemsAmount() - 1), curItem + 1);
        }
        curMenu->processInput();
    }
}