#include <mbed.h>
#include <./../lib/Omni/Omni.hpp>
#include<vector>
#define DATA_LEN 15

enum Keys{
  X,Y,R
};
enum eTrackingState{
        SYSTEM_NOT_READY=-1,
        NO_IMAGES_YET=0,
        NOT_INITIALIZED=1,
        OK=2,
        LOST=3
};
DigitalOut led(LED3);

Omni mecanum(4);
PwmOut wheel_pwm[] = {
  A6,A1,D7,D2
};
DigitalOut wheel_dir[4][2] = {
  {A4,A5},{A2,A3},{D6,D5},{D4,D3}
};
Timer LoopTimer;
Serial pc(USBTX,USBRX);

float duty[4] = {};
float velocityVector[3] = {};
float current_angle = -90.0f*3.14f/180.0f;

float rec_buf[DATA_LEN] = {};
int rec_index = 0;
bool rec_found = false;
bool ems = true;

//現在地
vector<float> npos(3,0);

int dir = 0;
//カメラが自分の位置を見失っていないか
int frame_init = eTrackingState::SYSTEM_NOT_READY;

//受信
void Receive();
//受信データの解析
void Decode(char keys);

int main() {
  wheel_pwm[0].period_us(500);
  pc.baud(9600);
  // put your setup code here, to run once:

  while(1) {
    LoopTimer.start();

    for(int i=0; i<3; i++) velocityVector[i] = 0.0f;
    Receive();

    if(!ems){
      if(frame_init == eTrackingState::OK){
        velocityVector[1] = 0.3f;
      }
      else
      {
        velocityVector[1] = -0.3f;
      }
      
      if(npos[2] < -10){
        velocityVector[2] = 0.1f;
      }
      else if(npos[2] > 10){
        velocityVector[2] = -0.1f;
      }
    }


    mecanum.calculate(velocityVector, duty, current_angle);

    for(int i=0; i<4; i++){
      wheel_dir[i][0] = (duty[i] < 0)? 1:0;
      wheel_dir[i][1] = (duty[i] < 0)? 0:1;
      if(abs(duty[i]) > 1.0f)
        duty[i] = 1.0f;

      if(duty[i] < 0)
        duty[i] *= -1.0f;
      wheel_pwm[i] = duty[i];
      // wheel_pwm[i] = 1.0f;
      // wheel_dir[i][0] = 0;
      // wheel_dir[i][1] = 1;
    }
    // pc.printf("0:%f, 1:%f, 2:%f, 3:%f,\r\n",duty[0],duty[1], duty[2], duty[3]);
    // pc.printf("hello world\r\n");

    while(LoopTimer.read_us() < 500);
    LoopTimer.stop();
    LoopTimer.reset();  
  }
}

union RecConverter{float f; unsigned char uc[4];};

vector<unsigned char> data;
bool rec_enable = false;

void Receive(){
  RecConverter rec_pos[3];

  if (pc.readable())
  {
    unsigned char rec = pc.getc();
    if(rec == 0x80){
      rec_enable = true;
    }
    if(rec_enable)
      data.push_back(rec);
  }

    
  if(data.size() == DATA_LEN){
    led = !led.read();
    for(int j=0; j<3; j++)
      for(int i=0; i<4; i++){
        rec_pos[j].uc[i] = data[j*4 + i+1];
      }

    frame_init = data[data.size() - 2];
    ems = data[data.size() - 1];

    if(frame_init)
      for(int i=0; i<3; i++)
        npos[i] = rec_pos[i].f;
    
    data.clear();    
    rec_enable = false;
  }

}

void Decode(char keys){
  switch (keys)
  {
  case 'w':
    velocityVector[1] = 0.5f;
    led = 1;
    break;
  case 'a':
    velocityVector[0] = -0.5f;
    led = 1;
    break;
  case 's':
    velocityVector[1] = -0.5f;
    led = 1;
    break;
  case 'd':
    velocityVector[0] = 0.5f;
    led = 1;
    break;
  case 'u':
    for(int i=0; i<3; i++)
      velocityVector[i]=0.0f;
    break;
  default:
  led = 0;
    break;
  }
}