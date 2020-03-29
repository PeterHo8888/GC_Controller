#pragma once
#include <Nintendo.h>

class Menu {
private:
    CGamecubeController *controller;
    CGamecubeConsole *console;
    typedef void (*GC_func)(void);

    GC_func map[sizeof(uint8_t)] = {nullptr};
public:
    Menu(CGamecubeController *controller, CGamecubeConsole *c2);
    void set_item(uint8_t dpad, uint8_t button, GC_func f);
    void loop();

    static const int dpad_down;
    static const int dpad_left;
    static const int dpad_right;
    static const int dpad_up;

    static const int A;
    static const int B;
    static const int X;
    static const int Y;
};

const int Menu::dpad_left  = 1 << 0;
const int Menu::dpad_right = 1 << 1;
const int Menu::dpad_down  = 1 << 2;
const int Menu::dpad_up    = 1 << 3;

const int Menu::A = 1 << 0;
const int Menu::B = 1 << 1;
const int Menu::X = 1 << 2;
const int Menu::Y = 1 << 3;

Menu::Menu(CGamecubeController *c, CGamecubeConsole *c2)
{
    controller = c;
    console = c2;
}

void Menu::set_item(uint8_t dpad, uint8_t button, GC_func f)
{
    int8_t command = dpad << 4 | button;
    map[command] = f;
}

void Menu::loop()
{
    uint8_t dpad, button;
    uint16_t command;
    Gamecube_Report_t re = controller->getReport();

    dpad = re.dleft | re.dright | re.ddown | re.dup;
    button = re.a | re.b | re.x | re.y;

    command = dpad << 4 | button;

    if (button == 0)
        console->write(*controller);
    else if (map[command] != nullptr)
        map[command]();
}

