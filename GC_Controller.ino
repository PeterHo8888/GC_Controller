#include "menu.hh"
#include <Nintendo.h>

CGamecubeConsole console(8);
CGamecubeController controller(7);
Menu menu(&controller, &console);

static const Gamecube_Report_t empty;

static int step = 16;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    menu.set_item(Menu::dpad_up, Menu::A, mash);
    menu.set_item(Menu::dpad_down, Menu::A, crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch);
    menu.set_item(Menu::dpad_right, Menu::A, online_taunt);
    menu.set_item(Menu::dpad_left, Menu::A, SDI);

    while (!Serial);
    Serial.begin(115200);

    controller.read();
    mash();
}

void loop()
{
    menu.loop();
}

void change_speed(int x, int y)
{
    if (x)
        step -= 1;
    if (y)
        step += 1;
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
        deg += 0.1; // Quarter-circle every ~16 steps
        re = controller.getReport();
        delay(step);
    } while (!re.start);

    digitalWrite(LED_BUILTIN, 0);
}

void crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch()
{
    Gamecube_Report_t re;
    int frames = 0;
    float deg = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;
        frames++;
        if(frames < 7) {
            re.xAxis = 127; // go down
            re.yAxis = 0; 
        } else if(frames == 7) {
            re.xAxis = 230; //go right
            re.yAxis = 127;
        } else if(frames == 8) {
            re.xAxis = 30; //go left
            re.yAxis = 127;
            frames = 0;
        }
        console.write(re); 

        re = controller.getReport();
        delay(step);
    } while (!re.start);

    digitalWrite(LED_BUILTIN, 0);
}

void online_taunt()
{
    Gamecube_Report_t re;
    int frame = 0;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re.yAxis = 127;

        // Can be 2-5 frames with smoke, 1 without
        if (frame << 1 == 0)
            re.xAxis = 0;
        else if (frame << 1 == 1)
            re.xAxis = 255;

        console.write(re);

        if (++frame == 4)
            frame = 0;

        re = controller.getReport();
        delay(step);
    } while (!re.start);

    digitalWrite(LED_BUILTIN, 0);
}

void SDI()
{
    int frame = 0;
    Gamecube_Report_t re;
    digitalWrite(LED_BUILTIN, 1);
    do {
        if(frame && 1 == 1){
            re.xAxis = 127;
            re.yAxis = 127;
        }
        re = controller.getReport();
        frame++;
        delay(step);
    } while(!re.start);
    digitalWrite(LED_BUILTIN, 0);
}



void write_usb(Gamecube_Report_t *re)
{
    // abxys000 dLdRdDdUzrl1 jXjYcXcYlr
    Serial.write((char*)re, 8);
}

