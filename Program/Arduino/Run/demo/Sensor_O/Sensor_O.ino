//***********************************************************
//***  閾値の調整するために超音波センサのデータを可視化するプログラム
//***  YANG(2022/11/30) バージョン1.0
//***********************************************************
//超音波センサは，2cmから400cmまで測れる．
#define PIN_ECHO 2 //エコー，超音波センサ入力用の信号を受信
#define PIN_TRIG 3 //トリガー，超音波センサ出力用の信号を送信
#define PIN_PWR  4 //パワー5V

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode( PIN_ECHO, INPUT );
  pinMode( PIN_TRIG, OUTPUT);
  pinMode( PIN_PWR,  OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int Distance=0;//距離
  int Duration=0;//間隔時間
  
  digitalWrite(PIN_TRIG, LOW); 
  delay(2); 
  digitalWrite(PIN_TRIG, HIGH ); //超音波を出力
  delay( 10 ); //
  digitalWrite(PIN_TRIG, LOW );
  
  Duration = pulseIn(PIN_ECHO, HIGH ); //センサからの入力
  
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    Serial.print("Distance:");
    Serial.print(Distance);
    Serial.println("cm");
  }
  delay(500);
}
