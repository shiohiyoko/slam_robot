### 3輪オムニ、4輪オムニ用ライブラリ

#### 3輪オムニの場合
コンストラクタの\_wheel\_numに3を入れる。 
##### モーターの番号付
![Omni3](https://wiki.robo.suzuka.io/uploads/attachment/5a101c42768caa3258998332/ad2e99a253b43b4a0d784d8814663943.png)
fを0、lを1、rを2とする。
以下のサンプルコードでは、大きさ3のMotorポインタ型の配列を作成し、motor[0]がf、motor[1]がl、motor[2]がrとなっている。

#### 4輪オムニの場合
コンストラクタの\_wheel\_numに4を入れる。 
##### モーターの番号付
![Omni4](https://wiki.robo.suzuka.io/uploads/attachment/5a101c42768caa3258998332/4dcd993318289840179f6935fc12f088.png)  
rfを0、lfを1、lbを2、rbを3とする。(第一象限〜第四象限のように対応)

#### 関数について
##### void calculate(float velocityVector[], float _pwm[], float currentAngle=0);
- velocityVector[]: 速度ベクトルを格納した配列
- \_pwm[]: 各モーターへの出力がここに入る
- currentAngle: ロボットがフィールド座標系の基準から見たときの角度 

#### サンプルコード
```c++:3輪オムニの場合.cpp
#include "mbed.hpp
#include "Omni.hpp"
#include "Motor.hpp"
Omni omni(3);
Timer t;
int main() {
    float velocityVector[3]; //速度ベクトルを格納 (V_x, V_y, V_theta)
    
    /*詳しくはMotorライブラリの使い方を参照*/
    Motor *motor[3];
    motor[0] = new Motor(PA_8, PA_11);
    motor[1] = new Motor(PA_9, PA_10);
    motor[2] = new Motor(PB_5, PB_4);
    while(true){
    	t.start();
    	/*速度を決定*/
    	velocityVector[0] = 0.5; /*X軸方向*/
    	velocityVector[1] = 0.5; /*Y軸方向*/
    	velocityVector[2] = 0.5; /*角速度(超信地旋回)*/
    	float pwm[3] = {0};
    	/*配列の先頭のポインタを渡している、ライブラリ内部で値が書き換わる*/
      	omni.calculate(velocityVector, pwm, 0);
      	
      	for(size_t i=0; i<3; i++){
      		*motor[i] = pwm[i];
      	}
      	while(t.read_ms() <= 10);
	    t.stop();
	    t.reset();
	    }
    }

```
四輪オムニの場合も同様