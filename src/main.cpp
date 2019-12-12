#include <mbed.h>
#include <./../lib/Omni/Omni.hpp>
#include<vector>
#define DATA_LEN 4

enum Keys{
  X,Y,R
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

float npos[3]= {0.0,0.0,0.0};
int dir = 0;
bool frame_init = false;

void Receive();
void Decode(char keys);

int main() {
  wheel_pwm[0].period_us(500);
  pc.baud(115200);
  // put your setup code here, to run once:

  while(1) {
    LoopTimer.start();

    // for(int i=0; i<3; i++) velocityVector[i] = 0.0f;
    Receive();

    if(frame_init){
      velocityVector[1] = 0.5f;
    }
    else
    {
      velocityVector[1] = -0.5f;
    }
    
    if(npos[2] < -10){
      velocityVector[2] = 0.1f;
    }
    else if(npos[2] > 10){
      velocityVector[2] = -0.1f;
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

union a{float f; unsigned char uc[4];};

vector<unsigned char> data;
bool rec_enable = false;
void Receive(){
  a pos[3];

  while (pc.readable())
  {
    unsigned char rec = pc.getc();
    if(rec == 0x80){
      rec_enable = true;
      data.clear();
    }
    // pc.gets(rec,2);
    // pc.putc(pc.getc());
    // data.push_back(pc.getc());
    if(rec_enable)
      data.push_back(rec);
  
  }

    
  if(data.size() >= 0){
    led = !led.read();
    for(int i=0; i<3; i++){
      pos[i].uc[0] = data[i];
    }
    frame_init = data[6];

    for(auto c:data)
      pc.putc(c);
    
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