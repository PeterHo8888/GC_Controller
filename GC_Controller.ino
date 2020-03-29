#include "menu.hh"
#include <Nintendo.h>

CGamecubeConsole console(8);
CGamecubeController controller(7);
Menu menu(&controller);

static const Gamecube_Report_t empty;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    menu.set_item(Menu::dpad_down, Menu::A, mash);

    while (!Serial);
    Serial.begin(115200);

    Serial.println(F("Press any key."));
    while (Serial.read() == -1);

    Serial.println(F("Starting"));
    Serial.println();

    controller.read();
}

void loop()
{
    menu.loop();
}


void mash()
{
    int x, y;
    int led = 0;
    float deg = 0;
    Gamecube_Report_t re;
    do {
        x = 127 * cos(deg) + 127;
        y = 127 * sin(deg) + 127;
        re = empty;
        re.xAxis = x;
        re.yAxis = y;
        console.write(re);
        deg += 0.1; // Quarter-circle every ~16 steps
        digitalWrite(LED_BUILTIN, led = !led);
    } while (!controller.getReport().start);
}
