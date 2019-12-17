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
union RecConverter{float f; unsigned char uc[4];};

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
float angle_correction = -90.0f;

float rec_buf[DATA_LEN] = {};
int rec_index = 0;
bool rec_found = false;
bool ems = true;

//現在地
vector<float> npos(3,0);

int dir = 0;
//カメラが自分の位置を見失っていないか
int frame_init = eTrackingState::SYSTEM_NOT_READY;

float standard_angle = angle_correction;

vector<RecConverter> send_buf(4);


//受信
void Receive();
//送信
void Send();
//受信データの解析
void Decode(char keys);

int main() {
  pc.baud(9600);
  // put your setup code here, to run once:

  while(1) {
    LoopTimer.start();

    Receive();

    for(int i=0; i<3; i++) velocityVector[i] = 0.0f;

    if(!ems){
      if(frame_init == eTrackingState::OK){
        velocityVector[1] = 0.6f;
      
        if(npos[2] < standard_angle){
          velocityVector[2] = 0.1f;
        }
        else if(npos[2] > standard_angle){
          velocityVector[2] = -0.1f;
        }
      }
      else if(frame_init == eTrackingState::LOST)
      {
        velocityVector[1] = -0.6f;
      }
    }

    mecanum.calculate(velocityVector, duty, npos[2], false);

    for(int i=0; i<4; i++){
      wheel_dir[i][0] = (duty[i] < 0)? 1:0;
      wheel_dir[i][1] = (duty[i] < 0)? 0:1;
      if(abs(duty[i]) > 1.0f)
        duty[i] = 1.0f;

      if(duty[i] < 0)
        duty[i] *= -1.0f;
      wheel_pwm[i] = duty[i];
    }

    while(LoopTimer.read_ms() < 1);
    LoopTimer.stop();
    LoopTimer.reset();  
  }
}


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
    for(int j=0; j<3; j++)
      for(int i=0; i<4; i++){
        rec_pos[j].uc[i] = data[j*4 + i+1];
      }

    frame_init = data[data.size() - 2];
    ems = data[data.size() - 1];

    if(frame_init == eTrackingState::OK){
      for(int i=0; i<3; i++)
        npos[i] = rec_pos[i].f;
      npos[2] += angle_correction;
    }

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

void Send(){
  for(auto c:send_buf)
    for(auto a:c.uc)
      pc.putc(a);
}