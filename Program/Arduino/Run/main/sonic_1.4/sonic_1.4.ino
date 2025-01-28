//*****************************************************************
//*** ゴール探索でのセンサの通信とサーボモータコントロールのプログラム
//*** H8はPDS信号を受け，制御する．
//*** SONICからの信号を受けるArduinoがRCの回を制御する
//*** YANG(2022/12/14) バージョン1.4
//*****************************************************************
#include <Servo.h>

Servo RC1servo;
Servo RC2servo;

#define PIN_TRG_SONIC 2   //SONIC超音波センサのECHO入力
#define PIN_PWR_SONIC 4   //SONIC超音波センサへのパワー
#define PIN_ECHO_SONIC 3  //SONIC超音波センサのTRG出力

#define PIN_OUTPUT_RC1 9  //腕を挟む用RC1への信号線
#define PIN_OUTPUT_RC2 10 //磁石に作用するRC2への信号線

int interval=0;           //pulse
double distance=0;        //距離
double sum=0;             //距離総和

int i=0;                  //距離平均のため
int i_s=0;                //距離判断のため
int RC1_pos=0;            //RC1の角度
int RC2_pos=0;            //RC2の角度

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_TRG_SONIC, OUTPUT);
  pinMode(PIN_ECHO_SONIC, INPUT);

  RC1servo.attach(PIN_OUTPUT_RC1);
  RC2servo.attach(PIN_OUTPUT_RC2);
  
  Serial.begin(115200); //シリアル通信のボートを開く

  digitalWrite(PIN_TRG_SONIC,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  int sonic_r = 5;//超音波センサの閾値の値，この値によりRC1の運転を停止する.
  
  //超音波センサ
    // 距離計測
    //trigger pulse output (10us)
    digitalWrite(PIN_TRG_SONIC, HIGH);
    delayMicroseconds(9);
    digitalWrite(PIN_TRG_SONIC, LOW);
    // echo pulse width measure
    interval = pulseIn(PIN_ECHO_SONIC, HIGH, 30000);  // 時間計測
    distance = interval * 0.017;  
    // 時間=>距離に変換 [cm]340[m/sec]/2 = 0.017[cm/μs]

    if(i<100){
      sum = sum + distance;
      i+=1;
      //総和を求め
      delay(20);  // adjustable  
    }else{
      distance = sum/100;
      //平均を求める
      i=0;
      sum=0;
      //初期化する
      
      // send result
      Serial.print("pulse width =");
      Serial.print(interval);
      Serial.print("[us] distance =");
      Serial.print(distance);
      Serial.print("[cm]\n");
      
      i_s = rc(distance, sonic_r);

      Serial.print("i_s=");
      Serial.print(i_s);
      Serial.print("\n");
      
      //RC1の回転
      if( (RC1_pos <= 70) && (i_s == 0) ){
        Serial.print("RC1回転中\n"); 
        RC1servo.write(RC1_pos);
        RC1_pos+=5;    
        delay(50);                
      }

      if()
      
      //RC2の回転
      if( (RC1_pos >=70 ) && (i_s == 1) ){
        Serial.print("RC2回転中");
        for(int i=0; RC2_pos<180;RC2_pos+=1 ){
          RC2servo.write(RC2_pos);    
          delay(15);           
        }
      }
    }    
}

int rc(double distance,int sonic_r){
      if( distance >= sonic_r){
        Serial.print("RC1回転指令\n");
        return 0;
      }else{
        Serial.print("RC1停止指令\n");        
        return 1;
      }
}
