#include "menu.hh"
#include <Nintendo.h>

CGamecubeConsole console(6);
CGamecubeController controller(7);
Menu menu(&controller, &console);

static const Gamecube_Report_t empty = defaultGamecubeData.report;


extern "C" {
    void print(const char name[], uint8_t val) {
        Serial.print(name);
        Serial.print(": ");
        Serial.println(val);
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    menu.set_item(Menu::dpad_left, Menu::B, mash);
    menu.set_item(Menu::dpad_down, Menu::B,
        crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch);
    menu.set_item(Menu::dpad_up, Menu::B, smokeless_taunt);
    menu.set_item(Menu::dpad_right, Menu::B, salty_rage_quit);

    menu.set_item(Menu::dpad_down, Menu::A, diddy_infinite);
    menu.set_item(Menu::dpad_left, Menu::A, stutter_walk);

    while (!Serial);
    Serial.begin(115200);

    while (!controller.read()) {
        delay(20);
    }
}

void loop()
{
    menu.loop();
}

inline bool cancel(Gamecube_Report_t re)
{
    return re.left < 128 && re.right < 128 && !re.x && !re.y;
}

void mash()
{
    Gamecube_Report_t re;
    int frame = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;
        if (frame < 2) {
            re.xAxis = 255;
        }
        else if (frame < 4) {
            re.xAxis = 0;
        }
        if (++frame == 4)
            frame = 0;
        console.write(re);
        controller.read();
        re = controller.getReport();
    } while (cancel(re));

    digitalWrite(LED_BUILTIN, 0);
}

void crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch()
{
    Gamecube_Report_t re;
    int frame = 0;
    int dir = 1;

    const int crouch = 15;
    const int turn = 3;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;

        ++frame;
        if (frame < crouch) {
            re.yAxis = 0;
        } else if (frame < crouch+turn) {
            re.xAxis = 128-40*dir;
        } else if (frame < crouch+2*turn) {
            re.xAxis = 128+40*dir;
        } else {
            re.yAxis = 0;
            frame = 0;
            dir = -dir;
        }

        console.write(re);
        controller.read();
        re = controller.getReport();
    } while (cancel(re));

    digitalWrite(LED_BUILTIN, 0);
}

void smokeless_taunt()
{
    Gamecube_Report_t re;
    int frame = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;
        ++frame;
        if (frame < 6) {
            re.xAxis = 255;
        }
        else if (frame < 10) {
            re.xAxis = 0;
        } else {
            frame = 0;
        }
        console.write(re);
        controller.read();
        re = controller.getReport();
    } while (cancel(re));

    digitalWrite(LED_BUILTIN, 0);
}

void salty_rage_quit()
{
    Gamecube_Report_t re;
    digitalWrite(LED_BUILTIN, 1);
    int frame = 0;
    do {
        ++frame;
        re = empty;
        if (frame < 10) {
            re.start = 1;
        } else if (frame < 30) {
            re.left = 255;
            re.right = 255;
            re.a = 1;
            re.start = 1;
        } else if (frame < 35) {
            re.xAxis = 255;
        } else if (frame < 1000) {
            re.a = frame & 1;
        }
        console.write(re);
    } while (frame < 1000);

    digitalWrite(LED_BUILTIN, 0);
}

void diddy_infinite()
{
    Gamecube_Report_t re;
    digitalWrite(LED_BUILTIN, 1);

    int frame = 0;
    int dir = 1;
    do {
        /*
        10 110 115 170 180
        */
        re = empty;
        if (frame < 10) {             // pluck banana
            re.b = 1;
            re.yAxis = 0;
            re.xAxis = 128 - 10*dir;
        } else if (frame < 110) {     // jump
            re.y = 1;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 115) {     // grab
            re.z = 1;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 170) {     // throw down
            re.cyAxis = 0;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 180) {     // drop through
            re.yAxis = 0;
        } else {
            frame = 0;
            dir = -dir;
        }
        ++frame;
        console.write(re);
        controller.read();
        re = controller.getReport();
    } while (cancel(re));

    digitalWrite(LED_BUILTIN, 0);
}

void stutter_walk()
{
    Gamecube_Report_t re;
    digitalWrite(LED_BUILTIN, 1);

    int frame = 0;
    do {
        re = empty;
        ++frame;
        if (frame < 8) {
            re.xAxis = 128 + 30;
        } else if (frame < 16) {
            re.xAxis = 128;
        } else {
            frame = 0;
        }
        console.write(re);
        controller.read();
        re = controller.getReport();
    } while (cancel(re));

    digitalWrite(LED_BUILTIN, 0);
}
