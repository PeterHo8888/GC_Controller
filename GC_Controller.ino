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
    menu.set_item(Menu::dpad_up, Menu::B, salty_rage_quit);

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

void mash()
{
    Gamecube_Report_t re;
    int x, y;
    float deg = 0;
    //digitalWrite(LED_BUILTIN, 1);
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
        digitalWrite(LED_BUILTIN, 0);
    } while (!re.start);

}

void crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch()
{
    Gamecube_Report_t re;
    int frames = 0;
    float deg = 0;
    
    const int crouch = 15;
    const int turn = 3;
    digitalWrite(LED_BUILTIN, 1);
    do {
        re = empty;
        re.cxAxis = 128;
        re.cyAxis = 128;
        
        frames++;
        if (frames >= 0 && frames < crouch) {
            re.xAxis = 128; // go down
            re.yAxis = 0;
        } else if (frames >= crouch && frames < crouch+turn) {
            re.xAxis = 128-40; //go left
            re.yAxis = 128;
        } else if (frames >= crouch+turn && frames < crouch+2*turn) {
            re.xAxis = 128+40; //go right
            re.yAxis = 128;
        } else if (frames >= crouch+2*turn && frames < 2*crouch+2*turn) {
            re.xAxis = 128; // go down
            re.yAxis = 0;
        } else if (frames >= 2*crouch+2*turn && frames < 2*crouch+3*turn) {
            re.xAxis = 128+40; //go right
            re.yAxis = 128;
        } else if (frames >= 2*crouch+3*turn && frames < 2*crouch+4*turn) {
            re.xAxis = 128-40; //go left
            re.yAxis = 128;
        } else {
            re.xAxis = 128; // go down
            re.yAxis = 0;
            frames = 0;
        }
        
        console.write(re); 
        controller.read();
        re = controller.getReport();
    } while (!re.a);

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
        re = controller.getReport();
        if(frame && 1 == 1){
            re.xAxis = 127;
            re.yAxis = 127;
        }
        console.write(re);
        frame++;
        delay(step);
    } while(!re.start);
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
