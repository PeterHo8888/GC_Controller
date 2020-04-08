#include "menu.hh"
#include <Nintendo.h>

CGamecubeConsole console(8);
CGamecubeController controller(7);
Menu menu(&controller, &console);

static const Gamecube_Report_t empty = defaultGamecubeData.report;

static int step = 16;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    menu.set_item(Menu::dpad_left, Menu::B, mash);
    menu.set_item(Menu::dpad_down, Menu::B,
        crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch);
    menu.set_item(Menu::dpad_up, Menu::B, online_taunt);
    menu.set_item(Menu::dpad_right, Menu::B, salty_rage_quit);
    
    menu.set_item(Menu::dpad_down, Menu::A, diddy_infinite);

    while (!Serial);
    Serial.begin(115200);
    //while (Serial.read() == -1);

    while (!controller.read()) {
        Serial.println("error reading controller.");
        delay(20);
    }
    Serial.println("Starting");
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
    int x, y;
    float deg = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        x = 127 * cos(deg) + 127;
        y = 127 * sin(deg) + 127;
        re = empty;
        re.xAxis = x;
        re.yAxis = y;
        console.write(re);
        deg += 90; // Quarter-circle every ~16 steps
        if (deg > 360)
            deg = 0;
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
        if (frame >= 0 && frame < crouch) {
            re.xAxis = 128; // go down
            re.yAxis = 0;
        } else if (frame < crouch+turn) {
            re.xAxis = 128-40*dir; //go left
            re.yAxis = 128;
        } else if (frame < crouch+2*turn) {
            re.xAxis = 128+40*dir; //go right
            re.yAxis = 128;
        } else {
            re.xAxis = 128; // go down
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

void online_taunt()
{
    Gamecube_Report_t re;
    int frame = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;
        ++frame;
        if (frame < 6) {
            re.xAxis = 255;
            re.yAxis = 128;
        }
        else if (frame < 10) {
            re.xAxis = 0;
            re.yAxis = 128;
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
    re.start = 1;
    console.write(re);
    delay(step);
    re = empty;
    re.l = 1;
    re.r = 1;
    re.a = 1;
    re.start = 1;
    console.write(re);
    delay(step);
    re = empty;
    re.xAxis = 255;
    re.yAxis = 127;
    console.write(re);
    delay(step);
    re = empty;
    re.a = 1;
    console.write(re);
    delay(step);
    digitalWrite(LED_BUILTIN, 0);
}

void diddy_infinite()
{
    Gamecube_Report_t re;
    digitalWrite(LED_BUILTIN, 1);
    
    int frame = 0;
    int dir = 1;
    do {
        re = empty;
        if (frame < 10) {
            re.b = 1;
            re.yAxis = 0;
            re.xAxis = 128 - 10*dir;
        } else if (frame < 110) {
            re.y = 1;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 120) {
            re.z = 1;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 170) {
            re.cyAxis = 0;
            re.xAxis = 128 + 50*dir;
        } else if (frame < 180) {
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
        
    
}
