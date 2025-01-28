//*****************************************************************
//*** ゴール探索でのセンサの通信とサーボモータコントロールのプログラム
//*** PSDからの信号を受けるArduinoがH8へ送信し，側面に設置される．車庫入れが終わった後，一回目直線運動
//*** SONICからの信号を受けるArduinoがH8へ送信，正面に設置される．二回目直線運動
//*** YANG(2022/12/30) バージョン1.1
//*****************************************************************

#include <Servo.h>

Servo RC1servo;
Servo RC2servo;

#define PIN_TRG_SONIC 2     //SONIC超音波センサのECHO入力
#define PIN_PWR_SONIC 4     //SONIC超音波センサへのパワー
#define PIN_ECHO_SONIC 3    //SONIC超音波センサのTRG出力

#define PIN_OUTPUT_RC1 9    //腕を挟む用RC1への信号線
#define PIN_OUTPUT_RC2 10   //磁石に作用するRC2への信号線

#define PIN_INPUT_PSD 5     //PSDセンサからの入力
#define PIN_OUTPUT_PSD 6    //処理したPSDの信号をH8へ送る
#define PIN_OUTPUT_SONIC 7  //処理したPSDの信号をH8へ送る

int interval=0;           //pulse
double distance=0;        //距離
double sum=0;             //距離総和

int i=0;                  //距離平均のため
int i_s=0;                //距離判断のため
int RC1_pos=0;            //RC1の角度
int RC2_pos=0;            //RC2の角度
int i_PSD= 1;             //PSDセンサの実行
int PSD_r = 50;             //PSDの閾値

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_TRG_SONIC, OUTPUT);
  pinMode(PIN_ECHO_SONIC, INPUT);
  pinMode(PIN_OUTPUT_PSD, OUTPUT);
  pinMode(PIN_OUTPUT_SONIC, OUTPUT);

  RC1servo.attach(PIN_OUTPUT_RC1);
  RC2servo.attach(PIN_OUTPUT_RC2);
  
  Serial.begin(115200); //シリアル通信のボートを開く

  digitalWrite(PIN_TRG_SONIC,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  int sonic_r = 5;//超音波センサの閾値の値，この値によりRC1の運転を停止する.

  int PSD_val = PSDread(PIN_INPUT_PSD);
  
  if(i_PSD == 1){
    //PSD処理
    digitalWrite(PIN_OUTPUT_PSD,LOW);
    
    Serial.print(PSD_val);
    Serial.print("  ");
    if( AnaToCm(PSD_val)!=0 ){
      if(PSD_val <= PSD_r){
          digitalWrite(PIN_OUTPUT_PSD,HIGH);
          i_PSD =0; //PSDを閉じる,SONIcを開く
        }
      Serial.print(AnaToCm(PSD_val)-10);
      Serial.print("~");
      Serial.print(AnaToCm(PSD_val));
      Serial.println("cm");
    }else{
      digitalWrite(PIN_OUTPUT_PSD,HIGH);
      Serial.println("out of range");
    }
  
  }else{
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

      if(i_s == 0){
        digitalWrite(PIN_OUTPUT_SONIC,LOW);
      }else{
        digitalWrite(PIN_OUTPUT_SONIC,HIGH);
      }
  
      //時間を持つ
      
      Serial.print("i_s=");
      Serial.print(i_s);
      Serial.print("\n");
      
      //RC1の回転
      if(i_s == 1){
        Serial.print("RC1回転中\n"); 
        RC1servo.write(RC1_pos);
        RC1_pos+=5;    
        delay(50);                
      }
      
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
}

int rc(double distance,int sonic_r){
      if( distance >= sonic_r){
        Serial.print("直線運転指令\n");
        return 0;
      }else{
        Serial.print("直線運運転停止指令\n");        
        return 1;
      }
}

int PSDread(int PinNo){ //PSDの読み取り
  long ans=0;
 int i;
 for(i=0; i<100; i++){
  ans=ans +analogRead(PinNo);
 }
 return ans/100;
}

int AnaToCm(int analogValue){ //PSDの距離示す
  if(analogValue<85) return(0);
  if(analogValue>=470) return(10);
  if(analogValue>=260) return(20);
  if(analogValue>=165) return(30);
  if(analogValue>=126) return(40);
  if(analogValue>=105) return(50);
  if(analogValue>=85) return(60);
  return(-1);
}
