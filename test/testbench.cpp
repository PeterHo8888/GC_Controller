#include <iostream>
#include <cstdint>
#include <cmath>

using namespace std;

typedef struct {
    // first data byte (bitfields are sorted in LSB order)
    uint8_t a : 1;
    uint8_t b : 1;
    uint8_t x : 1;
    uint8_t y : 1;
    uint8_t start : 1;
    uint8_t high0 : 1;
    uint8_t errlatch : 1;
    uint8_t errstat : 1;

    // second data byte
    uint8_t dleft : 1;
    uint8_t dright : 1;
    uint8_t ddown : 1;
    uint8_t dup : 1;
    uint8_t z : 1;
    uint8_t r : 1;
    uint8_t l : 1;
    uint8_t high1 : 1;

    // 3rd-8th data byte
    uint8_t xAxis;
    uint8_t yAxis;
    uint8_t cxAxis;
    uint8_t cyAxis;
    uint8_t left;
    uint8_t right;
} Gamecube_Report_t;

static const Gamecube_Report_t empty = {0};

class controller{
public:	
    Gamecube_Report_t report = {0};

    controller(){
        report.start = 0;
        report.y = 0;
        report.x = 0;
        report.b = 0;
        report.a = 0;
        report.l = 0;
        report.r = 0;
        report.z = 0;
        report.dup = 0;
        report.dright = 0;
        report.ddown = 0;
        report.dleft = 0;
        report.xAxis = 0;
        report.yAxis = 0;
        report.cxAxis = 0;
        report.cyAxis = 0;
        report.left = 0;
        report.right = 0;
    }

    

    void getValues(){
        cout << "START: " << (int)report.start << endl;	
        cout << "Y: " << (int)report.y << endl;		
        cout << "X: " << (int)report.x << endl;		
        cout << "B: " << (int)report.b << endl;
        cout << "A: " << (int)report.a << endl;	
        cout << "L: " << (int)report.l << endl;	
        cout << "R: " << (int)report.r << endl;	
        cout << "Z: " << (int)report.z << endl;
        cout << "D_UP: " << (int)report.dup << endl;
        cout << "D_RIGHT: " << (int)report.dright << endl;		
        cout << "D_DOWN: " << (int)report.ddown << endl;
        cout << "D_LEFT: " << (int)report.dleft << endl;
        cout << "xAXIS: " << (int)report.xAxis << endl;
        cout << "yAXIS: " << (int)report.yAxis << endl;
        cout << "cxAXIS: " << (int)report.cxAxis << endl;
        cout << "cyAXIS: " << (int)report.cyAxis << endl;
        cout << "LEFT: " << (int)report.left << endl;
        cout << "RIGHT: " << (int)report.right << endl;
    }

};

void mash(controller &Controller)
{
    int x, y;
    int led = 0;
    float deg = 0;
    do {
        x = 127 * cos(deg) + 127;
        y = 127 * sin(deg) + 127;
        Controller.report = empty;
        Controller.report.xAxis = x;
        Controller.report.yAxis = y;
        Controller.getValues();
        deg += 0.1; // Quarter-circle every ~16 steps
    } while (getchar() == '\n');
}

// steps: 
// crouch for 6 frames
// face right for 1 frame
// pinpoint the x cooridanate between the absolute value of 25 and 60 to the left

void crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch(controller &Controller){
    int x, y;
    int frames = 0;
    float deg = 0;

    do {
        frames++;
        if(frames < 7){
            Controller.report.xAxis = 112.5; // go down
            Controller.report.yAxis = 0; 
            Controller.getValues();
        }
        
        if(frames == 7){
            Controller.report.xAxis = 255; //go right
            Controller.report.yAxis = 112.5;
            Controller.getValues(); 
        }


        if(frames == 8){
            Controller.report.xAxis = 30; //go left
            Controller.report.yAxis = 112.5;
            Controller.getValues();
            frames = 0;
        }

    } while(getchar() == '\n');

}


int main(){

    controller Controller;

    cout << "Press enter to advance a frame." << endl;
    crouch_cancelled_walk_cancelled_turnaround_cancelled_crouch(Controller);

    return 0;
}
