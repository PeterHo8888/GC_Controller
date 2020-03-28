#include <iostream>
#include <cstdint>

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
    }

};


class gameCube{
public:
    gameCube(){

    }

};




int main(){

    controller Controller;

    Controller.report.a = 0;

    Controller.getValues();


    return 0;
}